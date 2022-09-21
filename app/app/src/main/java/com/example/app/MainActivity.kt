package com.example.app

import android.app.Activity
import android.os.Bundle
import android.util.Log
import android.view.Window
import android.widget.EditText
import android.widget.SeekBar
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.JsonObjectRequest
import com.android.volley.toolbox.Volley
import org.json.JSONObject

class MainActivity : Activity() {


    private lateinit var seekThrottle: SeekBar
    private lateinit var seekSteering: SeekBar
    private lateinit var editTextEspIp: EditText

    private var homeURL = "http://"
    private lateinit var queue: RequestQueue


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        this.requestWindowFeature(Window.FEATURE_NO_TITLE)
        queue = Volley.newRequestQueue(this)
        setContentView(R.layout.activity_main)
    }

    override fun onPostCreate(savedInstanceState: Bundle?) {
        super.onPostCreate(savedInstanceState)
        seekThrottle = findViewById(R.id.seekThrottle)
        seekSteering = findViewById(R.id.seekSteering)
        editTextEspIp = findViewById(R.id.editTextEspIp)

        editTextEspIp.setOnFocusChangeListener { _, hasFocus ->
            if (!hasFocus) {
                this.homeURL = "http://" + editTextEspIp.text.toString()
            }
        }

        seekThrottle.max = 210
        seekThrottle.progress = 105
        seekThrottle.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                val throttle = progress - 105       // throttle is a value between -100 and 100
                sendHttpRequest("throttle", throttle)
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                // reset the seekbar to 100 if the user stops touching it
                seekThrottle.progress = 105
            }
        })
        seekSteering.max = 80
        seekSteering.progress = 40
        seekSteering.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                val steering = seekSteering.max - progress      // turning is a value between 0 and 180
                sendHttpRequest("steering", steering)
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                // reset the seekbar to 100 if the user stops touching it
                seekSteering.progress = 90
            }
        })

        seekSteering.max = 180
        seekSteering.progress = 90

    }

    private fun sendHttpRequest(command: String, value: Int) {
        val url = "$homeURL/controls/$command"

        val params = HashMap<Any?, Any?>()
        params["value"] = value
        val jsonObject = JSONObject(params)

        val request = JsonObjectRequest(Request.Method.POST, url, jsonObject,
            { response ->
                Log.d("MainActivity", "Response: $response")
            },
            { error ->
                Log.d("MainActivity", "Error: $error")
            }
        )
        queue.add(request)
    }
}