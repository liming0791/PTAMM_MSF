package at.jku.ptam;

import android.app.Activity;
import android.hardware.SensorManager;
import android.util.Log;

import com.citrus.slam.MahonyAHRS.QuaternionEventListener;
import com.citrus.slam.MahonyAHRS.QuaternionSensor;

/**
 * Created by liming on 17-2-14.
 */
public class SensorController implements QuaternionEventListener {


    private QuaternionSensor mQuaternionSensor;
    private PTAM ptam;

    private float val[] = new float[6];
    private float q[] = new float[4];

    private Object qLock = new Object();

    public SensorController(Activity activity, PTAM ptam){
        super();

        SensorManager manager = (SensorManager)(activity.getSystemService(activity.SENSOR_SERVICE));
        mQuaternionSensor = new QuaternionSensor(manager);
        mQuaternionSensor.registerListener(this);

        this.ptam = ptam;
    }

    public void onStop() {
        mQuaternionSensor.unregisterListener(this);
    }

    @Override
    public void onQuaternionEvent(float[] q) {
        synchronized (qLock) {
            this.q[0] = q[0];
            this.q[1] = q[1];
            this.q[2] = q[2];
            this.q[3] = q[3];
        }
        //Log.i("SensorController", "q: " + q[0] + " " + q[1] + " " + q[2] + " " + q[3]);
    }

    @Override
    public void onIMUvalEvent(float[] imuval) {
        this.val[0] = imuval[0];
        this.val[1] = imuval[1];
        this.val[2] = imuval[2];
        this.val[3] = imuval[3];
        this.val[4] = imuval[4];
        this.val[5] = imuval[5];

        ptam.predictIMU(this.val);
    }

    public float[] getQuaternion() {
        float data[] = new float[4];
        synchronized (qLock) {
            data[0] = q[0];
            data[1] = q[1];
            data[2] = q[2];
            data[3] = q[3];
        }
        return data;
    }
}
