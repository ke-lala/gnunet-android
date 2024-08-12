package org.gnu.gnunet

import android.content.res.AssetManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import org.gnu.gnunet.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI(assets)
    }

    /**
     * A native method that is implemented by the 'gnunet' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(assets: AssetManager): String

    companion object {
        // Used to load the 'gnunet' library on application startup.
        init {
            System.loadLibrary("sqliteX");
            System.loadLibrary("gnunetti")
        }
    }
}