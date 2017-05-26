package org.iotivity.base.examples;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.nfc.NfcAdapter;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.EditText;

import org.iotivity.base.ErrorCode;
import org.iotivity.base.ModeType;
import org.iotivity.base.ObserveType;
import org.iotivity.base.OcConnectivityType;
import org.iotivity.base.OcException;
import org.iotivity.base.OcHeaderOption;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.OcRepresentation;
import org.iotivity.base.OcResource;
import org.iotivity.base.OcResourceIdentifier;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;

import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import android.widget.ImageButton;
import android.widget.Toast;

/**
 * Created by wonhee on 5/11/17.
 */

public class Open extends Activity {
    public static Context mContext;
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open);
        mContext = this;

        ImageButton btn = (ImageButton) findViewById(R.id.OpenBTN);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((SimpleClient)SimpleClient.mContext).getLightResourceRepresentation();
            }
        });
    }




    public void toastMessage(){
        Toast toast = Toast.makeText(getApplicationContext(), "Unlocked", Toast.LENGTH_SHORT);
        toast.show();

    }
    public void errorMessage(){
        Toast toast = Toast.makeText(getApplicationContext(), "Unlock Failed", Toast.LENGTH_SHORT);
        toast.show();
    }

    public synchronized void onGetCompleted(List<OcHeaderOption> list,
                                            OcRepresentation ocRepresentation) {

        toastMessage();
    }

    public synchronized void onGetFailed(Throwable throwable) {
        errorMessage();

    }

}