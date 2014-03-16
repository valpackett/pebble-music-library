package com.floatboth.pml.data;

public class Song {
  public long mSongId;
  public String mSongName;
  public String mArtistName;
  public String mAlbumName;
  public int mDuration;

  public Song(final long songId, final String songName, final String artistName, final String albumName, final int duration) {
    mSongId = songId;
    mSongName = songName;
    mArtistName = artistName;
    mAlbumName = albumName;
    mDuration = duration;
  }
}
