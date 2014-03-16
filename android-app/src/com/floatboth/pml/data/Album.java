package com.floatboth.pml.data;

public class Album {
  public long mAlbumId;
  public String mAlbumName;
  public String mArtistName;
  public int mSongNumber;
  public String mYear;
  public Album(final long albumId, final String albumName, final String artistName,
      final int songNumber, final String albumYear) {
    super();
    mAlbumId = albumId;
    mAlbumName = albumName;
    mArtistName = artistName;
    mSongNumber = songNumber;
    mYear = albumYear;
  }
}
