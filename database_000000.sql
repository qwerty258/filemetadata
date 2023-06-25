CREATE TABLE IF NOT EXISTS files (
    file_id INTEGER PRIMARY KEY,
    file_name TEXT DEFAULT "",
    file_size INTEGER DEFAULT 0,
    file_sha1sum TEXT DEFAULT "");

CREATE TABLE IF NOT EXISTS tags (
    tag_id INTEGER PRIMARY KEY,
    tag_name TEXT DEFAULT "");

CREATE TABLE IF NOT EXISTS tag_file_join (
    tag_id INTEGER,
    file_id INTEGER);

CREATE TABLE IF NOT EXISTS meta_books (
    book_id INTEGER PRIMARY KEY,
    author TEXT NOT NULL DEFAULT "",
    secondary_author TEXT NOT NULL DEFAULT "",
    subsidiary_author TEXT NOT NULL DEFAULT "",
    year TEXT NOT NULL DEFAULT "",
    title TEXT NOT NULL DEFAULT "",
    secondary_title TEXT NOT NULL DEFAULT "",
    pages TEXT NOT NULL DEFAULT "",
    volume TEXT NOT NULL DEFAULT "",
    number TEXT NOT NULL DEFAULT "",
    number_of_volumes TEXT NOT NULL DEFAULT "",
    place_published TEXT NOT NULL DEFAULT "",
    publisher TEXT NOT NULL DEFAULT "",
    edition TEXT NOT NULL DEFAULT "",
    keywords TEXT NOT NULL DEFAULT "",
    abstract TEXT NOT NULL DEFAULT "",
    url TEXT NOT NULL DEFAULT "",
    tertiary_title TEXT NOT NULL DEFAULT "",
    tertiary_author TEXT NOT NULL DEFAULT "",
    isbn TEXT NOT NULL DEFAULT "",
    chinese_unified_book_number TEXT NOT NULL DEFAULT "",
    alternate_title TEXT NOT NULL DEFAULT "",
    accession_number TEXT NOT NULL DEFAULT "",
    call_number TEXT NOT NULL DEFAULT "",
    short_title TEXT NOT NULL DEFAULT "",
    section TEXT NOT NULL DEFAULT "",
    original_publication TEXT NOT NULL DEFAULT "",
    reprint_edition TEXT NOT NULL DEFAULT "",
    electronic_resource_number TEXT NOT NULL DEFAULT "",
    translated_author TEXT NOT NULL DEFAULT "",
    translated_title TEXT NOT NULL DEFAULT "",
    language TEXT NOT NULL DEFAULT "",
    record_properties TEXT NOT NULL DEFAULT "",
    record_last_updated INTEGER NOT NULL DEFAULT 0);

CREATE TABLE IF NOT EXISTS meta_books_file_join (
    book_id INTEGER,
    file_id INTEGER);

CREATE TABLE IF NOT EXISTS meta_serials (
    serial_id INTEGER PRIMARY KEY,
    author TEXT NOT NULL DEFAULT "",
    year TEXT NOT NULL DEFAULT "",
    title TEXT NOT NULL DEFAULT "",
    journal TEXT NOT NULL DEFAULT "",
    volume TEXT NOT NULL DEFAULT "",
    issue TEXT NOT NULL DEFAULT "",
    pages TEXT NOT NULL DEFAULT "",
    start_page TEXT NOT NULL DEFAULT "",
    issn TEXT NOT NULL DEFAULT "",
    doi TEXT NOT NULL DEFAULT "",
    keywords TEXT NOT NULL DEFAULT "",
    abstract TEXT NOT NULL DEFAULT "",
    author_address TEXT NOT NULL DEFAULT "",
    language TEXT NOT NULL DEFAULT "");

CREATE TABLE IF NOT EXISTS meta_serials_file_join (
    serial_id INTEGER,
    file_id INTEGER);

CREATE TABLE IF NOT EXISTS meta_torrents (
    torrent_id INTEGER PRIMARY KEY,
    name TEXT NOT NULL DEFAULT "",
    url TEXT NOT NULL DEFAULT "",
    publisher TEXT NOT NULL DEFAULT "",
    created_by TEXT NOT NULL DEFAULT "",
    creation_date TEXT NOT NULL DEFAULT "",
    piece_length INTEGER DEFAULT 0,
    info_hash_v1 TEXT NOT NULL DEFAULT "",
    info_hash_v2 TEXT NOT NULL DEFAULT "");

CREATE TABLE IF NOT EXISTS files_in_torrent (
    length INTEGER DEFAULT 0,
    path TEXT NOT NULL DEFAULT "",
    torrent_id INTEGER DEFAULT NULL);

CREATE TABLE IF NOT EXISTS meta_torrents_file_join (
    torrent_id INTEGER,
    file_id INTEGER);

CREATE TABLE IF NOT EXISTS version (
    version_id INTEGER);

INSERT INTO version (version_id) VALUES (0);
