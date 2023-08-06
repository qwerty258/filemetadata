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
    author TEXT,
    secondary_author TEXT,
    subsidiary_author TEXT,
    year TEXT,
    title TEXT,
    secondary_title TEXT,
    pages TEXT,
    volume TEXT,
    number TEXT,
    number_of_volumes TEXT,
    place_published TEXT,
    publisher TEXT,
    edition TEXT,
    keywords TEXT,
    abstract TEXT,
    url TEXT,
    tertiary_title TEXT,
    tertiary_author TEXT,
    isbn TEXT,
    chinese_unified_book_number TEXT,
    alternate_title TEXT,
    accession_number TEXT,
    call_number TEXT,
    short_title TEXT,
    section TEXT,
    original_publication TEXT,
    reprint_edition TEXT,
    electronic_resource_number TEXT,
    translated_author TEXT,
    translated_title TEXT,
    language TEXT);
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
    author TEXT,
    year TEXT,
    title TEXT,
    journal TEXT,
    volume TEXT,
    issue TEXT,
    pages TEXT,
    start_page TEXT,
    issn TEXT,
    doi TEXT,
    keywords TEXT,
    abstract TEXT,
    author_address TEXT,
    language TEXT);
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
