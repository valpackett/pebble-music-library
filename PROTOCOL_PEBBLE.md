# Pebble Music Library - AppMessage protocol

This document describes the messages between the Pebble app and the phone app.

## Getting Artists

Pebble -> Phone

- `MSG_TYPE` (0) = `REQ` (0)  
  `MSG_CTX` (1) = `ARTISTS` (0)  

Phone -> Pebble

- `MSG_TYPE` (0) = `RSP_START` (1)  
  `MSG_CTX` (1) = `ARTISTS` (0)  
  `COUNT` (100) = 2  
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `ARTISTS` (0)  
  `INDEX` (101) = 0  
  `ID` (102) = 12345  
  `NAME` (103) = "Paramore"
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `ARTISTS` (0)  
  `INDEX` (101) = 1  
  `ID` (102) = 54321  
  `NAME` (103) = "Placebo"

## Getting Albums

Pebble -> Phone

- `MSG_TYPE` (0) = `REQ` (0)  
  `MSG_CTX` (1) = `ALBUMS` (1)  

or

- `MSG_TYPE` (0) = `REQ` (0)  
  `MSG_CTX` (1) = `ALBUMS` (1)  
  `ID` (102) = 12345

where `ID` is the artist ID

Phone -> Pebble

- `MSG_TYPE` (0) = `RSP_START` (1)  
  `MSG_CTX` (1) = `ALBUMS` (1)  
  `COUNT` (100) = 2
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `ALBUMS` (1)  
  `INDEX` (101) = 0  
  `ID` (102) = 150  
  `NAME` (103) = "All We Know Is Falling"
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `ALBUMS` (1)  
  `INDEX` (101) = 1  
  `ID` (102) = 125  
  `NAME` (103) = "RIOT!"

## Getting Songs from an Album

Pebble -> Phone

- `MSG_TYPE` (0) = `REQ` (0)  
  `MSG_CTX` (1) = `SONGS` (3)  
  `MSG_PARENT_CTX` (2) = `ALBUMS` (1)  
  `ID` (102) = 125  

Phone -> Pebble

- `MSG_TYPE` (0) = `RSP_START` (1)  
  `MSG_CTX` (1) = `SONGS` (3)  
  `COUNT` (100) = 2
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `SONGS` (3)  
  `INDEX` (101) = 0  
  `ID` (102) = 78345  
  `NAME` (103) = "When It Rains"
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `SONGS` (3)  
  `INDEX` (101) = 1  
  `ID` (102) = 35627  
  `NAME` (103) = "Let the Flames Begin"

## Playing Songs from an Album

Pebble -> Phone

- `MSG_TYPE` (0) = `PLAY` (4)  
  `MSG_PARENT_CTX` (2) = `ALBUMS` (1)  
  `INDEX` (101) = 1 (zero-based index of song in album)  
  `ID` (102) = 125 (album id)

## Getting Playlists

Pebble -> Phone

- `MSG_TYPE` (0) = `REQ` (0)  
  `MSG_CTX` (1) = `PLAYLISTS` (2)  

Phone -> Pebble

- `MSG_TYPE` (0) = `RSP_START` (1)  
  `MSG_CTX` (1) = `PLAYLISTS` (2)  
  `COUNT` (100) = 2  
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `PLAYLISTS` (2)  
  `INDEX` (101) = 0  
  `ID` (102) = 12345  
  `NAME` (103) = "Running"
- `MSG_TYPE` (0) = `RSP_DATA` (2)  
  `MSG_CTX` (1) = `PLAYLISTS` (2)  
  `INDEX` (101) = 1  
  `ID` (102) = 54321  
  `NAME` (103) = "Favorite Songs"

## Playing Playlists

Pebble -> Phone

- `MSG_TYPE` (0) = `PLAY` (4)  
  `MSG_CTX` (2) = `PLAYLISTS` (2)  
  `ID` (102) = 12345 (playlist id)
