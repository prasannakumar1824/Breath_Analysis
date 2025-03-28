import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt
import librosa
import librosa.display

def plot_spectrogram(audio_file):
    # Load audio file
    y, sr = librosa.load(audio_file, sr=None)
    
    # Compute the spectrogram
    S = librosa.stft(y)
    S_db = librosa.amplitude_to_db(np.abs(S), ref=np.max)
    
    # Plot the spectrogram
    plt.figure(figsize=(10, 4))
    librosa.display.specshow(S_db, sr=sr, x_axis='time', y_axis='log')
    plt.colorbar(format='%+2.0f dB')
    plt.title('Spectrogram')
    plt.xlabel('Time (s)')
    plt.ylabel('Frequency (Hz)')
    plt.show()
    
    return y, sr

def analyze_breath_pattern(y, sr):
    # Apply a bandpass filter (Typical breathing frequency: ~0.2-2 Hz)
    sos = signal.butter(4, [0.2, 2], btype='bandpass', fs=sr, output='sos')
    filtered = signal.sosfilt(sos, y)
    
    # Detect peaks (inhale/exhale cycles)
    peaks, _ = signal.find_peaks(filtered, distance=sr//2)
    breath_rate = len(peaks) / (len(y) / sr) * 60  # Convert to breaths per minute
    
    # Estimate breath capacity (approx. amplitude)
    breath_capacity = np.max(filtered) - np.min(filtered)
    
    # Determine possible respiratory conditions
    if breath_rate < 12:
        condition = "Possible Bradypnea (Abnormally slow breathing)"
    elif breath_rate > 20:
        condition = "Possible Tachypnea (Abnormally fast breathing)"
    else:
        condition = "Normal breathing rate"
    
    print(f'Estimated Breath Rate: {breath_rate:.2f} breaths per minute')
    print(f'Estimated Breath Capacity: {breath_capacity:.2f}')
    print(f'Diagnosis: {condition}')
    
    return breath_rate, breath_capacity, condition

# Example usage
audio_file = 'breath_sample.wav'  # Replace with your recorded file
y, sr = plot_spectrogram(audio_file)
analyze_breath_pattern(y, sr)

