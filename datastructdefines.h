#ifndef DATASTRUCTDEFINES_H
#define DATASTRUCTDEFINES_H

#include <QString>
#include <QVector>
#include <QDateTime>

typedef struct
{
    qint64 file_id;
    QString file_name;
    qint64 size;
    QString sha1;
} corrupted_file_t;

typedef enum
{
    METADATA_TYPE_BOOK,
    METADATA_TYPE_SERIAL,
    METADATA_TYPE_TORRENT,
    METADATA_TYPE_MAX
} metadata_type;

typedef struct
{
    QString path;
    quint64 length;
} file_in_torrent;

typedef struct
{
    QString comment;
    QString created_by;
    QString creation_date;
    QVector<file_in_torrent> files;
    QString info_hash_v1;
    QString info_hash_v2;
    QString name;
    quint64 piece_length;
    quint64 pieces;
} torrent_metadata_t;

typedef struct
{
    QString author;
    QString year;
    QString title;
    QString journal;
    QString volume;
    QString issue;
    QString pages;
    QString start_page;
    QString issn;
    QString doi;
    QString keywords;
    QString abstract;
    QString author_address;
    QString language;
} serial_metadata_t;

typedef struct
{
    QString author;
    QString secondary_author;
    QString subsidiary_author;
    QString year;
    QString title;
    QString secondary_title;
    QString pages;
    QString volume;
    QString number;
    QString number_of_volumes;
    QString place_published;
    QString publisher;
    QString edition;
    QString keywords;
    QString abstract;
    QString url;
    QString tertiary_title;
    QString tertiary_author;
    QString isbn;
    QString chinese_unified_book_number;
    QString alternate_title;
    QString accession_number;
    QString call_number;
    QString short_title;
    QString section;
    QString original_publication;
    QString reprint_edition;
    QString electronic_resource_number;
    QString translated_author;
    QString translated_title;
    QString language;
    QString record_properties;
    QString record_last_updated;
} book_metadata_t;

typedef struct
{
    metadata_type type;
    torrent_metadata_t torrent;
    serial_metadata_t serial;
    book_metadata_t book;
} metadata_t;

typedef struct
{
    QString full_path;
    QString file_name;
    QString sha1;
    qint64 size;
    metadata_t metadata;
} new_file_info_t;

#endif // DATASTRUCTDEFINES_H
