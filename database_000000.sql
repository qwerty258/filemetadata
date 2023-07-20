CREATE TABLE IF NOT EXISTS files (
    file_id INTEGER PRIMARY KEY,
    file_name TEXT DEFAULT "",
    file_size INTEGER DEFAULT 0,
    file_sha1sum TEXT DEFAULT "");
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS tags (
    tag_id INTEGER PRIMARY KEY,
    tag_name TEXT DEFAULT "" UNIQUE);
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS tag_file_join (
    tag_id INTEGER,
    file_id INTEGER,
    UNIQUE(tag_id, file_id) ON CONFLICT IGNORE);
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS tag_file_join_tag_deletion
    AFTER DELETE
    ON tags
BEGIN
    DELETE FROM tag_file_join WHERE tag_id=OLD.tag_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS tag_file_join_file_deletion
    AFTER DELETE
    ON files
BEGIN
    DELETE FROM tag_file_join WHERE file_id=OLD.file_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS books (
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
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS book_file_join (
    book_id INTEGER,
    file_id INTEGER,
    UNIQUE(book_id, file_id) ON CONFLICT IGNORE);
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS book_file_join_book_deletion
    AFTER DELETE
    ON books
BEGIN
    DELETE FROM book_file_join WHERE book_id=OLD.book_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS book_file_join_file_deletion
    AFTER DELETE
    ON files
BEGIN
    DELETE FROM book_file_join WHERE file_id=OLD.file_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS serials (
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
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS serial_file_join (
    serial_id INTEGER,
    file_id INTEGER,
    UNIQUE(serial_id, file_id) ON CONFLICT IGNORE);
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS serial_file_join_serial_deletion
    AFTER DELETE
    ON serials
BEGIN
    DELETE FROM serial_file_join WHERE serial_id=OLD.serial_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS serial_file_join_file_deletion
    AFTER DELETE
    ON files
BEGIN
    DELETE FROM serial_file_join WHERE file_id=OLD.file_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS torrents (
    torrent_id INTEGER PRIMARY KEY,
    comment TEXT,
    created_by TEXT,
    creation_date TEXT,
    info_hash_v1 TEXT,
    info_hash_v2 TEXT,
    name TEXT,
    piece_length INTEGER,
    pieces INTEGER);
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS files_in_torrent (
    torrent_id INTEGER,
    path TEXT,
    length INTEGER);
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS torrents_file_deletion
    AFTER DELETE
    ON files
BEGIN
    DELETE FROM torrents WHERE torrent_id=OLD.file_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TRIGGER IF NOT EXISTS files_in_torrent_torrent_deletion
    AFTER DELETE
    ON torrents
BEGIN
    DELETE FROM files_in_torrent WHERE torrent_id=OLD.torrent_id;
END;
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS version (
    version_id INTEGER);
/*STATEMENT SEPARATOR*/
INSERT INTO version (version_id) VALUES (0);
