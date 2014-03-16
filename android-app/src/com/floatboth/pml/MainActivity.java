package com.floatboth.pml;

import android.os.Bundle;
import android.app.Activity;

public class MainActivity extends Activity {

  MusicLibrary library;

  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);
    library = new MusicLibrary(this.getContentResolver());
  }

}
