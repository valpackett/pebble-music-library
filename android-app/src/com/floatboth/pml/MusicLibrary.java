package com.floatboth.pml;

import android.content.Context;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.BaseColumns;
import android.provider.MediaStore;
import android.provider.MediaStore.Audio.AudioColumns;
import android.provider.MediaStore.Audio.ArtistColumns;
import android.provider.MediaStore.Audio.AlbumColumns;
import android.provider.MediaStore.Audio.PlaylistsColumns;

import java.util.ArrayList;

import com.floatboth.pml.data.*;

// thanks:
// https://github.com/CyanogenMod/android_packages_apps_Apollo/tree/cm-11.0/src/com/andrew/apollo

public class MusicLibrary {

  ContentResolver resolver;

  MusicLibrary(ContentResolver resolver) {
    this.resolver = resolver;
  }

  private final Cursor makeArtistCursor() {
    return resolver.query(MediaStore.Audio.Artists.EXTERNAL_CONTENT_URI,
        new String[] {
          /* 0 */ BaseColumns._ID,
          /* 1 */ ArtistColumns.ARTIST,
          /* 2 */ ArtistColumns.NUMBER_OF_ALBUMS,
          /* 3 */ ArtistColumns.NUMBER_OF_TRACKS
        }, null, null, MediaStore.Audio.Artists.DEFAULT_SORT_ORDER);
  }

  public ArrayList<Artist> getArtists() {
    ArrayList<Artist> mArtistsList = new ArrayList<Artist>();
    Cursor mCursor = makeArtistCursor();
    if (mCursor != null && mCursor.moveToFirst()) {
      do {
        final long id = mCursor.getLong(0);
        final String artistName = mCursor.getString(1);
        final int albumCount = mCursor.getInt(2);
        final int songCount = mCursor.getInt(3);
        final Artist artist = new Artist(id, artistName, songCount, albumCount);
        mArtistsList.add(artist);
      } while (mCursor.moveToNext());
    }
    if (mCursor != null) {
      mCursor.close();
      mCursor = null;
    }
    return mArtistsList;
  }

  private final Cursor makeAlbumCursor() {
    return resolver.query(MediaStore.Audio.Albums.EXTERNAL_CONTENT_URI,
        new String[] {
          /* 0 */ BaseColumns._ID,
          /* 1 */ AlbumColumns.ALBUM,
          /* 2 */ AlbumColumns.ARTIST,
          /* 3 */ AlbumColumns.NUMBER_OF_SONGS,
          /* 4 */ AlbumColumns.FIRST_YEAR
        }, null, null, MediaStore.Audio.Albums.DEFAULT_SORT_ORDER);
  }

  private final Cursor makeArtistAlbumCursor(final Long artistId) {
    return resolver.query(MediaStore.Audio.Artists.Albums.getContentUri("external", artistId),
        new String[] {
          /* 0 */ BaseColumns._ID,
          /* 1 */ AlbumColumns.ALBUM,
          /* 2 */ AlbumColumns.ARTIST,
          /* 3 */ AlbumColumns.NUMBER_OF_SONGS,
          /* 4 */ AlbumColumns.FIRST_YEAR
        }, null, null, MediaStore.Audio.Albums.DEFAULT_SORT_ORDER);
  }

  public ArrayList<Album> getAlbums(final Long artistId) {
    ArrayList<Album> mAlbumList = new ArrayList<Album>();
    Cursor mCursor;
    if (artistId != null) {
      mCursor = makeArtistAlbumCursor(artistId);
    } else {
      mCursor = makeAlbumCursor();
    }
    if (mCursor != null && mCursor.moveToFirst()) {
      do {
        final long id = mCursor.getLong(0);
        final String albumName = mCursor.getString(1);
        final String artist = mCursor.getString(2);
        final int songCount = mCursor.getInt(3);
        final String year = mCursor.getString(4);
        final Album album = new Album(id, albumName, artist, songCount, year);
        mAlbumList.add(album);
      } while (mCursor.moveToNext());
    }
    if (mCursor != null) {
      mCursor.close();
      mCursor = null;
    }
    return mAlbumList;
  }

  private final Cursor makePlaylistCursor() {
    return resolver.query(MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI,
        new String[] {
          /* 0 */ BaseColumns._ID,
          /* 1 */ PlaylistsColumns.NAME
        }, null, null, MediaStore.Audio.Playlists.DEFAULT_SORT_ORDER);
  }

  public ArrayList<Playlist> getPlaylists() {
    ArrayList<Playlist> mPlaylistList = new ArrayList<Playlist>();
    Cursor mCursor = makePlaylistCursor();
    if (mCursor != null && mCursor.moveToFirst()) {
      do {
        final long id = mCursor.getLong(0);
        final String playlistName = mCursor.getString(1);
        final Playlist playlist = new Playlist(id, playlistName);
        mPlaylistList.add(playlist);
      } while (mCursor.moveToNext());
    }
    if (mCursor != null) {
      mCursor.close();
      mCursor = null;
    }
    return mPlaylistList;
  }

  private final Cursor makeArtistSongCursor(final Long artistId) {
    final StringBuilder selection = new StringBuilder();
    selection.append(AudioColumns.IS_MUSIC + "=1");
    selection.append(" AND " + AudioColumns.TITLE + " != ''");
    selection.append(" AND " + AudioColumns.ARTIST_ID + "=" + artistId);
    return resolver.query(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
        new String[] {
          /* 0 */ BaseColumns._ID,
          /* 1 */ AudioColumns.TITLE,
          /* 2 */ AudioColumns.ARTIST,
          /* 3 */ AudioColumns.ALBUM
        }, selection.toString(), null, MediaStore.Audio.Media.DEFAULT_SORT_ORDER);
  }

  public ArrayList<Song> getArtistSongs(final Long artistId) {
    ArrayList<Song> mSongList = new ArrayList<Song>();
    Cursor mCursor = makeArtistSongCursor(artistId);
    if (mCursor != null && mCursor.moveToFirst()) {
      do {
        final long id = mCursor.getLong(0);
        final String songName = mCursor.getString(1);
        final String artist = mCursor.getString(2);
        final String album = mCursor.getString(3);
        final Song song = new Song(id, songName, artist, album, -1);
        mSongList.add(song);
      } while (mCursor.moveToNext());
    }
    if (mCursor != null) {
      mCursor.close();
      mCursor = null;
    }
    return mSongList;
  }

  private final Cursor makeAlbumSongCursor(final Long albumId) {
    final StringBuilder selection = new StringBuilder();
    selection.append(AudioColumns.IS_MUSIC + "=1");
    selection.append(" AND " + AudioColumns.TITLE + " != ''");
    selection.append(" AND " + AudioColumns.ALBUM_ID + "=" + albumId);
    return resolver.query(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
        new String[] {
          /* 0 */ BaseColumns._ID,
          /* 1 */ AudioColumns.TITLE,
          /* 2 */ AudioColumns.ALBUM,
          /* 3 */ AudioColumns.ALBUM
        }, selection.toString(), null, MediaStore.Audio.Media.TRACK + ", " + MediaStore.Audio.Media.DEFAULT_SORT_ORDER);
  }

  public ArrayList<Song> getAlbumSongs(final Long albumId) {
    ArrayList<Song> mSongList = new ArrayList<Song>();
    Cursor mCursor = makeAlbumSongCursor(albumId);
    if (mCursor != null && mCursor.moveToFirst()) {
      do {
        final long id = mCursor.getLong(0);
        final String songName = mCursor.getString(1);
        final String artist = mCursor.getString(2);
        final String album = mCursor.getString(3);
        final Song song = new Song(id, songName, artist, album, -1);
        mSongList.add(song);
      } while (mCursor.moveToNext());
    }
    if (mCursor != null) {
      mCursor.close();
      mCursor = null;
    }
    return mSongList;
  }

}
