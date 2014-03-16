package com.floatboth.pml;

import android.app.IntentService;
import android.content.Intent;
import android.content.Context;
import android.util.Log;

import java.util.ArrayList;
import java.util.UUID;
import java.util.Queue;
import java.util.LinkedList;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.*;

public class PebbleSenderService extends IntentService {

  Queue<PebbleDictionary> queue = new LinkedList<PebbleDictionary>();

  @Override
  protected void onHandleIntent(Intent intent) {
    ArrayList<PebbleDictionary> messages = (ArrayList<PebbleDictionary>) intent.getSerializableExtra("messages");
    final UUID uuid = (UUID) intent.getSerializableExtra("uuid");
    for (PebbleDictionary message : messages) queue.offer(message);
    PebbleKit.sendDataToPebble(this, uuid, queue.poll());

    PebbleKit.registerReceivedAckHandler(getApplicationContext(), new PebbleKit.PebbleAckReceiver(uuid) {
      @Override
      public void receiveAck(Context context, int transactionId) {
        Log.i("PebbleSenderService", "Received ack for transaction " + transactionId);
        try { Thread.sleep(50); } catch (Exception ex) {}
        PebbleDictionary msg = queue.poll();
        if (msg != null) PebbleKit.sendDataToPebble(PebbleSenderService.this, uuid, msg);
      }
    });

    PebbleKit.registerReceivedNackHandler(getApplicationContext(), new PebbleKit.PebbleNackReceiver(uuid) {
      @Override
      public void receiveNack(Context context, int transactionId) {
        Log.i("PebbleSenderService", "Received nack for transaction " + transactionId);
        queue = new LinkedList<PebbleDictionary>();
      }
    });
  }

  public PebbleSenderService() {
    super("Pebble Sender Service");
  }
}
