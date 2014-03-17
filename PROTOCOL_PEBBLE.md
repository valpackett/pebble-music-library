# Pebble Music Library - AppMessage protocol

This document describes the messages between the Pebble app and the phone app.

## Getting Artists

Pebble -> Phone

- `MSG_TYPE` (0) = `GET_ARTISTS` (10)

Phone -> Pebble

- `MSG_TYPE` (0) = `START_ARTISTS` (11)  
  `COUNT` (100) = 2
- `MSG_TYPE` (0) = `SEND_ARTISTS` (12)  
  `INDEX` (101) = 0  
  `ID` (102) = 12345  
  `NAME` (103) = "Paramore"
- `MSG_TYPE` (0) = `SEND_ARTISTS` (12)  
  `INDEX` (101) = 1  
  `ID` (102) = 54321  
  `NAME` (103) = "Placebo"

## Getting Albums

Pebble -> Phone

- `MSG_TYPE` (0) = `GET_ALBUMS` (20)

or

- `MSG_TYPE` (0) = `GET_ALBUMS` (20)  
  `ID` (102) = 12345

where `ID` is the artist ID

Phone -> Pebble

- `MSG_TYPE` (0) = `START_ALBUMS` (21)  
  `COUNT` (100) = 2
- `MSG_TYPE` (0) = `SEND_ALBUMS` (22)  
  `INDEX` (101) = 0  
  `ID` (102) = 150  
  `NAME` (103) = "All We Know Is Falling"
- `MSG_TYPE` (0) = `SEND_ALBUMS` (22)  
  `INDEX` (101) = 1  
  `ID` (102) = 125  
  `NAME` (103) = "RIOT!"

## Getting Songs

Pebble -> Phone

- `MSG_TYPE` (0) = `GET_ALBUM_SONGS` (48)  
  `ID` (102) = 125

Phone -> Pebble

- `MSG_TYPE` (0) = `START_SONGS` (41)  
  `COUNT` (100) = 2
- `MSG_TYPE` (0) = `SEND_SONGS` (42)  
  `INDEX` (101) = 0  
  `ID` (102) = 78345  
  `NAME` (103) = "When It Rains"
- `MSG_TYPE` (0) = `SEND_SONGS` (42)  
  `INDEX` (101) = 1  
  `ID` (102) = 35627  
  `NAME` (103) = "Let the Flames Begin"

## Playing Songs

Pebble -> Phone

- `MSG_TYPE` (0) = `PLAY_SONG` (90)
  `INDEX` (101) = 1 (zero-based index of song in album)
  `ID` (102) = 125 (album id)

