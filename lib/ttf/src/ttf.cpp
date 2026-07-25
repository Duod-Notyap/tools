#include "util/byte_parse.hxx"
#include "ttf/ttf.h"

#include <algorithm>
#include <iomanip>

namespace ttf {

ttf_tag::ttf_tag() {}
ttf_tag::ttf_tag(const std::string& table) {
    if(table.length() != 4) {
        throw std::runtime_error{"Table tag must be 4 characters"};
    }

    chars.char1 = table[0];
    chars.char2 = table[1];
    chars.char3 = table[2];
    chars.char4 = table[3];
}

bool ttf_tag::operator==(const ttf_tag& other) const {
    return this->chars.char1 == other.chars.char1 && 
        this->chars.char2 == other.chars.char2 && 
        this->chars.char3 == other.chars.char3 && 
        this->chars.char4 == other.chars.char4;
}

ttf_tag::operator std::string() const { return this->str(); }
std::string ttf_tag::str() const { return std::string{name, 4}; }
std::ostream& operator<<(std::ostream& out, const ttf_tag& tag) {
    out << tag.str();
    return out;
}
    
ttf_table_record ttf_table_record::read(std::istream& in) {
    ttf_table_record header;

    header.tag.chars.char1 = ::read<char>(in);
    header.tag.chars.char2 = ::read<char>(in);
    header.tag.chars.char3 = ::read<char>(in);
    header.tag.chars.char4 = ::read<char>(in);
    header.checksum = read_be<uint32_t>(in);
    header.offset = read_be<uint32_t>(in);
    header.length = read_be<uint32_t>(in);

    return header;
}

size_t ttf_table_record::write(std::ostream& out) const {
    out.write(tag.name, 4);
    
    auto beChecksum = std::byteswap(checksum);
    auto beOffset = std::byteswap(offset);
    auto beLength = std::byteswap(length);

    out.write((char*)&beChecksum, sizeof(beChecksum));
    out.write((char*)&beOffset, sizeof(beOffset));
    out.write((char*)&beLength, sizeof(beLength));

    return sizeof(tag) + sizeof(checksum) + sizeof(offset) + sizeof(length);
}

std::ostream& operator<<(std::ostream& out, const ttf_table_record& record) {
    out << "tag: " 
        << record.tag.chars.char1 << record.tag.chars.char2 
        << record.tag.chars.char3 << record.tag.chars.char4
        << "\tchecksum: " << std::hex << std::setfill('0') << std::setw(8) << record.checksum << std::setw(0) << std::dec
        << "\toffset: " << record.offset 
        << "\tlength: " << record.length;

    return out;
}

ttf_header ttf_header::read(std::istream& in) {
    ttf_header header;

    header.sfntVersion = read_be<uint32_t>(in);
    header.numTables = read_be<uint16_t>(in);
    header.searchRange = read_be<uint16_t>(in);
    header.entrySelector = read_be<uint16_t>(in);
    header.rangeShift = read_be<uint16_t>(in);

    return header;
}

size_t ttf_header::write(std::ostream& out) const {
    auto beSfnt = std::byteswap(sfntVersion);
    auto beCount = std::byteswap(numTables);
    auto beSearch = std::byteswap(searchRange);
    auto beEntry = std::byteswap(entrySelector);
    auto beRange = std::byteswap(rangeShift);

    out.write((char*)&beSfnt, sizeof(beSfnt))
        .write((char*)&beCount, sizeof(beCount))
        .write((char*)&beSearch, sizeof(beSearch))
        .write((char*)&beEntry, sizeof(beEntry))
        .write((char*)&beRange, sizeof(beRange));

    return sizeof(beSfnt) + sizeof(beCount) + sizeof(beSearch) + sizeof(beEntry) + sizeof(beRange);
}

std::ostream& operator<<(std::ostream& out, const ttf_header& header) {
    out << "sfntVersion: " << header.sfntVersion << '\t'
        << "numTables: " << header.numTables << '\t'
        << "searchRange: " << header.searchRange << '\t'
        << "entrySelector: " << header.entrySelector << '\t'
        << "rangeShift: " << header.rangeShift;

    return out;
}

ttf_file::ttf_file() {
    table_records = new ttf_table_record[0];
    table_data = new char*[0];
}

void ttf_file::removeTable(int index) {
    if(index > header.numTables) {
        throw std::out_of_range{"index > header.numTables"};
    }

    auto* oldRecordsPtr = table_records;
    auto* oldDataPtr = table_data;
    auto oldSize = header.numTables--;

    table_records = new ttf_table_record[header.numTables];
    table_data = new char*[header.numTables];

    int _i = 0;
    for(int i = 0; i < oldSize; i++) {
        if(i == index) continue;

        table_records[_i] = oldRecordsPtr[i];
        table_data[_i] = oldDataPtr[i];

        _i++;
    }

    delete[] oldRecordsPtr;
    delete[] oldDataPtr[index];
    delete[] oldDataPtr;
}

void ttf_file::removeTable(ttf_tag table) {
    for(int i = 0; i < header.numTables; i++) {
        if(table_records[i].tag == table) {
            removeTable(i);
            return;
        }
    }
}

size_t ttf_file::write(std::ostream& out) const {
    size_t written = 0;

    written += header.write(out);
    for(int i = 0; i < header.numTables; i++) {
        written += table_records[i].write(out);
    }

    for(int i = 0; i < header.numTables; i++) {
        ttf_table_record& table = table_records[i];

        if(written < table.offset) {
            size_t toWrite = table.offset - written;

            std::cerr << "Writing " << toWrite << " bytes of filler before table " << table.tag << '\n';

            char dummy[toWrite];
            out.write(dummy, toWrite);
            written += toWrite;
        }

        if(written > table.offset) {
            std::stringstream ss;
            ss << "Table " << table.tag << " wants offset " << table.offset
                << " but " << written << " bytes have already been written";
            throw std::runtime_error{ss.str()};
        }

        out.write(table_data[i], table.length);
        written += table.length;
    }

    return written;
}

ttf_file::~ttf_file() {
    if(table_records != nullptr) {
        delete[] table_records;
    } 

    if(table_data != nullptr) {
        for(int i = 0; i < header.numTables; i++) {
            delete[] table_data[i];
        }
    } 
}

void ttf_read(std::istream& in, ttf_file *file) {
    file->header = ttf_header::read(in);
    file->table_records = new ttf_table_record[file->header.numTables];
    file->table_data = new char*[file->header.numTables];

    for(int i = 0; i < file->header.numTables; i++) {
        file->table_records[i] = ttf_table_record::read(in);
    }

    std::sort(file->table_records, file->table_records + file->header.numTables, [](ttf_table_record a, ttf_table_record b) {
        return ((long)a.offset - (long)b.offset) < 0;
    });

    size_t readBytes = sizeof(ttf_header) + sizeof(ttf_table_record)*file->header.numTables;
    for(int i = 0; i < file->header.numTables; i++) {
        ttf_table_record& table = file->table_records[i];

        size_t toSkip = table.offset - readBytes;
        if(toSkip > 0) {
            std::cerr << "Skipping " << toSkip << " bytes before table " << std::string{table.tag.name, 4} << '\n';

            char dummy[toSkip];
            in.read(dummy, toSkip);
            readBytes += toSkip;
        }

        char *buf = new char[table.length];
        in.read(buf, table.length);
        readBytes += table.length;

        file->table_data[i] = buf;
    }
}

} /* ttf */
