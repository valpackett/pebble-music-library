package com.floatboth.pml.data;

public class Artist {
  public long mArtistId;
  public String mArtistName;
  public int mAlbumNumber;
  public int mSongNumber;

  public Artist(final long artistId, final String artistName, final int songNumber, final int albumNumber) {
    super();
    mArtistId = artistId;
    mArtistName = artistName;
    mSongNumber = songNumber;
    mAlbumNumber = albumNumber;
  }

}
