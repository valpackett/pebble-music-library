package com.floatboth.pml;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.provider.MediaStore.Audio.Albums;
import android.provider.MediaStore.Audio.Playlists;

import java.util.UUID;
import java.util.ArrayList;
import com.google.common.primitives.Ints;
import com.google.common.primitives.Shorts;

import com.floatboth.pml.data.*;
import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.*;

public class PmlBroadcastReceiver extends PebbleKit.PebbleDataReceiver {

  private static final UUID PEBBLE_APP_UUID = UUID.fromString("5b51cba8-7750-441e-a012-f41510348434");
  // Keys
  private static final int MSG_TYPE = 0;
  private static final int MSG_CTX = 1;
  private static final int MSG_PARENT_CTX = 2;
  private static final int COUNT = 100;
  private static final int INDEX = 101;
  private static final int ID = 102;
  private static final int NAME = 103;
  // Values
  private static final short REQ = 0;
  private static final short RSP_START = 1;
  private static final short RSP_DATA = 2;
  private static final short RSP_END = 3; // unused
  private static final short PLAY = 4;
  private static final short ARTISTS = 0;
  private static final short ALBUMS = 1;
  private static final short PLAYLISTS = 2;
  private static final short SONGS = 3;

  public PmlBroadcastReceiver() {
    this.subscribedUuid = PEBBLE_APP_UUID;
  }

  private int getInteger(final PebbleDictionary data, int key) {
    try {
      return Ints.checkedCast(data.getUnsignedInteger(key));
    } catch (RuntimeException ex) {
      try {
        return Ints.checkedCast(data.getInteger(key));
      } catch (RuntimeException e) {
        return -1;
      }
    }
  }

  @Override
  public void receiveData(final Context context, final int transactionId, final PebbleDictionary data) {
    PebbleKit.sendAckToPebble(context, transactionId);
    Log.i("PmlBroadcastReceiver", "Got Pebble Msg");
    MusicLibrary library = new MusicLibrary(context.getContentResolver());
    int msgType = getInteger(data, MSG_TYPE);
    int msgCtx = getInteger(data, MSG_CTX);
    int msgParentCtx = getInteger(data, MSG_PARENT_CTX);
    if (msgType == REQ) {
      if (msgCtx == ARTISTS) {
        sendArtists(context, library);
      } else if (msgCtx == ALBUMS) {
        sendAlbums(context, library, data.getInteger(ID));
      } else if (msgCtx == SONGS && msgParentCtx == ALBUMS) {
        sendAlbumSongs(context, library, data.getInteger(ID));
      } else if (msgCtx == PLAYLISTS) {
        sendPlaylists(context, library);
      }
    } else if (msgType == PLAY) {
      if (msgParentCtx == ALBUMS) {
        playAlbumSong(context, library, data.getInteger(ID), data.getInteger(INDEX));
      } else if (msgCtx == PLAYLISTS) {
        playPlaylist(context, library, data.getInteger(ID));
      }
    }
  }

  private PebbleDictionary countMessage(final short context, final ArrayList entries) {
    PebbleDictionary countMsg = new PebbleDictionary();
    countMsg.addUint16(MSG_TYPE, RSP_START);
    countMsg.addUint16(MSG_CTX, context);
    countMsg.addUint16(COUNT, (short) entries.size());
    return countMsg;
  }

  private PebbleDictionary entryMessage(final short context, final short index, final long id, final String name) {
    PebbleDictionary entryMsg = new PebbleDictionary();
    entryMsg.addUint16(MSG_TYPE, RSP_DATA);
    entryMsg.addUint16(MSG_CTX, context);
    entryMsg.addUint16(INDEX, index);
    entryMsg.addUint32(ID, Ints.checkedCast(id));
    entryMsg.addString(NAME, name);
    return entryMsg;
  }

  private void startSending(final Context context, final ArrayList<PebbleDictionary> messages) {
    Intent intent = new Intent(context, PebbleSenderService.class);
    intent.putExtra("messages", messages);
    intent.putExtra("uuid", PEBBLE_APP_UUID);
    context.startService(intent);
  }

  private void sendArtists(final Context context, final MusicLibrary library) {
    ArrayList<PebbleDictionary> messages = new ArrayList<PebbleDictionary>();
    ArrayList<Artist> artists = library.getArtists();
    messages.add(countMessage(ARTISTS, artists));
    short index = 0;
    for (Artist artist : artists) {
      messages.add(entryMessage(ARTISTS, index, artist.mArtistId, artist.mArtistName));
      index += 1;
    }
    startSending(context, messages);
  }

  private void sendAlbums(final Context context, final MusicLibrary library, final Long artistId) {
    ArrayList<PebbleDictionary> messages = new ArrayList<PebbleDictionary>();
    ArrayList<Album> albums = library.getAlbums(artistId);
    messages.add(countMessage(ALBUMS, albums));
    short index = 0;
    for (Album album : albums) {
      messages.add(entryMessage(ALBUMS, index, album.mAlbumId, album.mAlbumName));
      index += 1;
    }
    startSending(context, messages);
  }

  private void sendAlbumSongs(final Context context, final MusicLibrary library, final Long albumId) {
    ArrayList<PebbleDictionary> messages = new ArrayList<PebbleDictionary>();
    ArrayList<Song> songs = library.getAlbumSongs(albumId);
    messages.add(countMessage(SONGS, songs));
    short index = 0;
    for (Song song : songs) {
      messages.add(entryMessage(SONGS, index, song.mSongId, song.mSongName));
      index += 1;
    }
    startSending(context, messages);
  }

  private void playAlbumSong(final Context context, final MusicLibrary library, final Long albumId, final Long position) {
    ArrayList<Song> songs = library.getAlbumSongs(albumId);
    Intent intent = new Intent(Intent.ACTION_VIEW);
    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    intent.setType(Albums.CONTENT_TYPE);
    intent.putExtra("album", albumId.toString());
    intent.putExtra("albumId", albumId);
    intent.putExtra("position", Ints.checkedCast(position));
    try {
      context.startActivity(intent);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  private void sendPlaylists(final Context context, final MusicLibrary library) {
    ArrayList<PebbleDictionary> messages = new ArrayList<PebbleDictionary>();
    ArrayList<Playlist> playlists = library.getPlaylists();
    messages.add(countMessage(PLAYLISTS, playlists));
    short index = 0;
    for (Playlist playlist : playlists) {
      messages.add(entryMessage(PLAYLISTS, index, playlist.mPlaylistId, playlist.mPlaylistName));
      index += 1;
    }
    startSending(context, messages);
  }

  private void playPlaylist(final Context context, final MusicLibrary library, final Long playlistId) {
    ArrayList<Playlist> playlists = library.getPlaylists();
    Intent intent = new Intent(Intent.ACTION_VIEW);
    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    intent.setType(Playlists.CONTENT_TYPE);
    intent.putExtra("playlist", playlistId.toString());
    intent.putExtra("playlistId", playlistId);
    try {
      context.startActivity(intent);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
