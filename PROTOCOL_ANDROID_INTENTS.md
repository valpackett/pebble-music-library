# Pebble Music Library - Android intents

This document describes the Intents the Android app sends to the player.
Use this if you're developing a music player that uses the phone's local library.
This protocol has been [implemented in Apollo](https://github.com/CyanogenMod/android_packages_apps_Apollo/commit/4e5a01cb19abd2cb549ddf371dabadf7456b4fde).

## Playing a Song in an Album

```java
Intent intent = new Intent(Intent.ACTION_VIEW); // android.intent.action.VIEW
intent.setType(android.provider.MediaStore.Audio.Albums.CONTENT_TYPE); // vnd.android.cursor.dir/albums
intent.putExtra("albumId", (Long) albumId);   // album id
intent.putExtra("album", albumId.toString()); // album id as string
intent.putExtra("position", (int) position);  // zero based index of song in album
```

## Playing a Song in an Artist

```java
Intent intent = new Intent(Intent.ACTION_VIEW); // android.intent.action.VIEW
intent.setType(android.provider.MediaStore.Audio.Artists.CONTENT_TYPE); // vnd.android.cursor.dir/artists
intent.putExtra("artistId", (Long) artistId);   // artist id
intent.putExtra("artist", artistId.toString()); // artist id as string
intent.putExtra("position", (int) position);    // zero based index of song in artist
```

## Playing a Playlist

```java
Intent intent = new Intent(Intent.ACTION_VIEW); // android.intent.action.VIEW
intent.setType(android.provider.MediaStore.Audio.Playlists.CONTENT_TYPE); // vnd.android.cursor.dir/playlist
intent.putExtra("playlistId", (Long) playlistId);   // playlist id
intent.putExtra("playlist", playlistId.toString()); // playlist id as string
```
