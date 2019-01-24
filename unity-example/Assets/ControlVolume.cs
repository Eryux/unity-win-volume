using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Slider))]
public class ControlVolume : MonoBehaviour {

    Slider slider;

    [DllImport("wvolume")]
    static extern float GetAudioVolume();

    [DllImport("wvolume")]
    static extern int SetAudioVolume(float v);

    void Start () {
        slider = GetComponent<Slider>();
        slider.value = GetAudioVolume();
	}
	
	public void SetVolume(Slider s)
    {
        float volume = s.value * 100.0f;
        SetAudioVolume(volume);
    }
}
