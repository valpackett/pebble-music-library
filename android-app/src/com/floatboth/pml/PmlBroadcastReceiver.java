package com.floatboth.pml;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.provider.MediaStore.Audio.Albums;

import java.util.UUID;
import java.util.ArrayList;
import com.google.common.primitives.Ints;
import com.google.common.primitives.Shorts;

import com.floatboth.pml.data.*;
import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.*;

public class PmlBroadcastReceiver extends PebbleKit.PebbleDataReceiver {

  private static final UUID PEBBLE_APP_UUID = UUID.fromString("5b51cba8-7750-441e-a012-f41510348434");
  private static final int MSG_TYPE = 0;
  private static final short GET_ARTISTS = 10;
  private static final short START_ARTISTS = 11;
  private static final short SEND_ARTISTS = 12;
  private static final short GET_ALBUMS = 20;
  private static final short START_ALBUMS = 21;
  private static final short SEND_ALBUMS = 22;
  private static final short GET_ALBUM_SONGS = 48;
  private static final short START_SONGS = 41;
  private static final short SEND_SONGS = 42;
  private static final short PLAY_ALBUM_SONG = 43;
  private static final int COUNT = 100;
  private static final int INDEX = 101;
  private static final int ID = 102;
  private static final int NAME = 103;

  public PmlBroadcastReceiver() {
    this.subscribedUuid = PEBBLE_APP_UUID;
  }

  @Override
  public void receiveData(final Context context, final int transactionId, final PebbleDictionary data) {
    PebbleKit.sendAckToPebble(context, transactionId);
    Log.i("YO", "Got Pebble Msg");
    MusicLibrary library = new MusicLibrary(context.getContentResolver());
    switch(Ints.checkedCast(data.getUnsignedInteger(MSG_TYPE))) {
      case GET_ARTISTS:
        sendArtists(context, library);
        break;
      case GET_ALBUMS:
        sendAlbums(context, library, data.getInteger(ID));
        break;
      case GET_ALBUM_SONGS:
        sendAlbumSongs(context, library, data.getInteger(ID));
        break;
      case PLAY_ALBUM_SONG:
        playAlbumSong(context, library, data.getInteger(ID), data.getInteger(INDEX));
        break;
    }
  }

  private PebbleDictionary countMessage(final short countType, final ArrayList entries) {
    PebbleDictionary countMsg = new PebbleDictionary();
    countMsg.addUint16(MSG_TYPE, countType);
    countMsg.addUint16(COUNT, (short) entries.size());
    return countMsg;
  }

  private PebbleDictionary entryMessage(final short entryType, final short index, final long id, final String name) {
    PebbleDictionary entryMsg = new PebbleDictionary();
    entryMsg.addUint16(MSG_TYPE, entryType);
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
    messages.add(countMessage(START_ARTISTS, artists));
    short index = 0;
    for (Artist artist : artists) {
      messages.add(entryMessage(SEND_ARTISTS, index, artist.mArtistId, artist.mArtistName));
      index += 1;
    }
    startSending(context, messages);
  }

  private void sendAlbums(final Context context, final MusicLibrary library, final Long artistId) {
    ArrayList<PebbleDictionary> messages = new ArrayList<PebbleDictionary>();
    ArrayList<Album> albums = library.getAlbums(artistId);
    messages.add(countMessage(START_ALBUMS, albums));
    short index = 0;
    for (Album album : albums) {
      messages.add(entryMessage(SEND_ALBUMS, index, album.mAlbumId, album.mAlbumName));
      index += 1;
    }
    startSending(context, messages);
  }

  private void sendAlbumSongs(final Context context, final MusicLibrary library, final Long albumId) {
    ArrayList<PebbleDictionary> messages = new ArrayList<PebbleDictionary>();
    ArrayList<Song> songs = library.getAlbumSongs(albumId);
    messages.add(countMessage(START_SONGS, songs));
    short index = 0;
    for (Song song : songs) {
      messages.add(entryMessage(SEND_SONGS, index, song.mSongId, song.mSongName));
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

}
