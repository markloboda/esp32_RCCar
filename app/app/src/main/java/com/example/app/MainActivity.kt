package com.example.app

import android.app.Activity
import android.os.Bundle
import android.util.Log
import android.view.Window
import android.widget.SeekBar
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.JsonObjectRequest
import com.android.volley.toolbox.Volley
import org.json.JSONObject

class MainActivity : Activity() {


    private lateinit var seekThrottle: SeekBar
    private lateinit var seekSteering: SeekBar

    private val homeURL = "http://192.168.64.107"
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

        seekThrottle.max = 200
        seekThrottle.progress = 100
        seekThrottle.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                val throttle = progress - 100       // throttle is a value between -100 and 100
                sendHttpRequest("throttle", throttle)
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                // reset the seekbar to 100 if the user stops touching it
                seekThrottle.progress = 100
            }
        })
        seekSteering.max = 200
        seekSteering.progress = 100
        seekSteering.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                val steering = progress - 100       // turning is a value between -100 and 100
                sendHttpRequest("steering", steering)
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                // reset the seekbar to 100 if the user stops touching it
                seekSteering.progress = 100
            }
        })

        seekSteering.max = 200
        seekSteering.progress = 100

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