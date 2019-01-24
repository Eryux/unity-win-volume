# unity-win-volume

Unity plugin to get and set main speaker volume on Windows.

### Usage

For usage only you can download pre-compiled plugin in release section.

* Add Plugins folder in Assets folder to your Unity project

* Use this following lines for importing plugin methods to your script

```csharp
[DllImport("wvolume")]
static extern float GetAudioVolume();

[DllImport("wvolume")]
static extern int SetAudioVolume(float v);
```

* Now you can use this methods in your script for get and set speakers volume

```csharp
void Start () {
  SetAudioVolume(50.0f)
  Debug.Log(GetAudioVolume()); // 0.5f
}
```

You can also take a look at Unity example inside unity-example folder.

### Build requirements

* Visual Studio 2015+ (minimun toolset v140)

### Build

* Open sln file with Visual Studio

* Select desired platform and build the solution

### License

MIT License
