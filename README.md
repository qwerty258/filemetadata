# FileMetadata

## Motivation

A cloud storage provider deleted a text file I uploaded when I was in college without my consent.

They can see your file if they want. Offline local storage is real privacy.

Never trust cloud storage provider, local data is really mine, but this doesn't prevent back up data to the cloud.

## Features

file and metadata management

 - [X] respect original file name and data integrity
 - [X] regard every file as binary data
 - [X] use sha1sum and size as integrity check
 - [X] file tags
 - [ ] book metadata
 - [X] serials metadata (journals, magazines, etc)
 - [X] torrent metadata
 - [ ] music metadata
 - [ ] picture/video metadata
 - [ ] import from friend's database
 - [ ] get all faces from images using opencv face recognition

## Data Directory

If you choose `~/Backup` in the settings.

`~/Backup/FileMetadata.db` is the sqlite database.

A file with SHA1 `9B50703BFD335D22590325E0D444E31AF339A2BB` will be stored as `~/Backup/9B/50/9B50703BFD335D22590325E0D444E31AF339A2BB.bin` in same directory of sqlite database.

Backup can be made easily by backup the whole directory of `~/Backup`.
