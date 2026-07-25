#pragma once
#ifndef INCLUDE_TTF_FONT_H_
#define INCLUDE_TTF_FONT_H_

#include <cstdint>
#include <iostream>

namespace ttf {
    
/**
 * @brief A tag for a table entry.
 */
union ttf_tag {
    char name[4];
    struct { char char1; char char2; char char3; char char4; } chars;

    /** @brief Create a zeroed out table tag. */
    ttf_tag();

    /** 
     * @brief Create a table tag from a string. The string MUST be exactly 4 
     * characters.
     */
    ttf_tag(const std::string& table);

    bool operator==(const ttf_tag& b) const;
    operator std::string() const;

    /**
     * @brief Get this tag as a string.
     */
    std::string str() const;

    friend std::ostream& operator<<(std::ostream& out, const ttf_tag& tag);
};


/**
 * @brief A table record entry.
 */
struct __attribute__((packed)) ttf_table_record {
    /** @brief Tag for the table */
    ttf_tag tag;

    /** @brief The table's checksum */
    uint32_t checksum;

    /** @brief The offset of the table from the beginning of the file */
    uint32_t offset;

    /** @brief The length of the table's data segment */
    uint32_t length;

    /**
     * @brief Read a new table record from an input stream.
     *
     * @param[in] in The input stream to read
     * @return The table record.
     */
    static ttf_table_record read(std::istream& in);

    /**
     * @brief Write the table record's bytes to a stream.
     *
     * @param[in] out The output stream to write to
     * @return The number of bytes written.
     */
    size_t write(std::ostream& out) const;

    /**
     * @brief Serialize basic information about this table.
     *
     * WARNING: Call write() to write the file data. This is for human readble data.
     */
    friend std::ostream& operator<<(std::ostream& out, const ttf_table_record& record);
};

/**
 * @brief The header at the beginning of every file.
 *
 */
struct __attribute__((packed)) ttf_header {
    uint32_t sfntVersion;

    /** @brief The number of tables in the file. */
    uint16_t numTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;

    /**
     * @brief Read a header from an input stream.
     *
     * @param[in] in Stream to read from 
     * @return A new ttf_header
     */
    static ttf_header read(std::istream& in);

    /**
     * @brief Write this header's bytes to an output stream.
     *
     * @param[in] out The output stream
     * @return The number of written bytes
     */
    size_t write(std::ostream& out) const;

    /**
     * @brief Serialize basic information about this header.
     *
     * WARNING: Call write() to write the file data. This is for human readble data.
     */
    friend std::ostream& operator<<(std::ostream& out, const ttf_header& header);
};

/**
 * struct ttf_file - A TTF file byte data.
 *
 * When using this class, ensure that you keep table_records, and table_data 
 * in the order they will appear in the file (they should be sorted so that 
 * lowest ttf_table_record::offset is first).
 */
struct ttf_file {
    /** 
     * The file header 
     */
    ttf_header header;

    /** 
     * Array of individual table headers. These should remain in the order 
     * they will appear in the finished file.
     *
     * Length: header.numTables
     */
    ttf_table_record *table_records;

    /** 
     * Array of table data segments. These should remain in the order they will 
     * appear in the finished file, matching the order of table_records.
     *
     * Length: header.numTables
     */
    char **table_data;

    /**
     * @brief Create a new TTF file. This will be fully zeroed out in every 
     * field.
     */
    ttf_file();

    // TODO
    ttf_file(const ttf_file& other) = delete;
    
    // TODO
    ttf_file& operator=(const ttf_file& other) = delete;

    /**
     * @brief Delete a table from the font file.
     *
     * @param[in] index The index of the table to delete;
     */
    void removeTable(int index);

    /**
     * @brief Delete a table from the font file.
     *
     * @param[in] table The tag of the table to delete;
     */
    void removeTable(ttf_tag table);

    /**
     * @brief Write a valid TTF file bytes to an output stream.
     *
     * @param[in] out The output stream
     * @return The number of written bytes.
     */
    size_t write(std::ostream& out) const;

    ~ttf_file();
};

/**
 * @brief Read a TTF file from an input stream into a instance.
 *
 * @param[in] in The input stream
 * @param[in] file A pointer to an empty ttf_file object
 */
void ttf_read(std::istream& in, ttf_file *file);

} /* ttf */

#endif  // INCLUDE_TTF_FONT_H_
