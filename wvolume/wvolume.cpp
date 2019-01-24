#include <iostream>

#include "wvolume.h"

/*
MIT License
Copyright (c) 2019, Candia Nicolas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

bool PickDevice(IMMDevice **DeviceToUse)
{
	HRESULT hr;

	bool retValue = true;

	IMMDeviceEnumerator *deviceEnumerator;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
	if (FAILED(hr)) {
		std::cerr << "Unable to instantiate device enumerator " << hr << std::endl;
		return false;
	}

	IMMDevice *device = nullptr;

	if (device == nullptr)
	{
		ERole deviceRole = eMultimedia;
		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, deviceRole, &device);
		if (FAILED(hr)) {
			std::cerr << "Unable to get default device for role " << deviceRole << " : " << hr << std::endl;
			SafeRelease(&deviceEnumerator);
			return false;
		}
	}

	*DeviceToUse = device;
	SafeRelease(&deviceEnumerator);
	return true;
}

float GetAudioVolume()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		std::cerr << "Unable to initialize COM: " << hr << std::endl;
		return 0.f;
	}

	IMMDevice *device = nullptr;
	if (!PickDevice(&device)) {
		return 0.f;
	}

	IAudioEndpointVolume *endpointVolume;
	hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, reinterpret_cast<void **>(&endpointVolume));
	if (FAILED(hr)) {
		std::cerr << "Unable to activate endpoint volume on output device: " << hr << std::endl;
		SafeRelease(&device);
		return 0.f;
	}

	float volume;
	hr = endpointVolume->GetMasterVolumeLevelScalar(&volume);
	if (FAILED(hr)) {
		std::cerr << "Unable to get volume: " << hr << std::endl;
		SafeRelease(&device);
		return 0.f;
	}

	SafeRelease(&device);
	CoUninitialize();

	return volume;
}

int SetAudioVolume(float v)
{
	if (v < 0.0f || v > 100.0f) {
		return -1;
	}

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		std::cerr << "Unable to initialize COM: " << hr << std::endl;
		return -2;
	}

	IMMDevice *device = nullptr;
	if (!PickDevice(&device)) {
		return -3;
	}

	IAudioEndpointVolume *endpointVolume;
	hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, reinterpret_cast<void **>(&endpointVolume));
	if (FAILED(hr)) {
		std::cerr << "Unable to activate endpoint volume on output device: " << hr << std::endl;
		SafeRelease(&device);
		return -4;
	}

	float newVolume = v / 100.0f;
	hr = endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
	if (FAILED(hr)) {
		std::cerr << "Unable to set volume: " << hr << std::endl;
		SafeRelease(&device);
		return -5;
	}

	SafeRelease(&device);
	CoUninitialize();

	return 0;
}