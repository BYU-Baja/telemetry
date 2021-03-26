from datetime import datetime
import matplotlib.pyplot as plt
import numpy as np


def is_outlier(points, thresh=3.5):
    """
    Returns a boolean array with True if points are outliers and False
    otherwise.

    Parameters:
    -----------
        points : An numobservations by numdimensions array of observations
        thresh : The modified z-score to use as a threshold. Observations with
            a modified z-score (based on the median absolute deviation) greater
            than this value will be classified as outliers.

    Returns:
    --------
        mask : A numobservations-length boolean array.

    References:
    ----------
        Boris Iglewicz and David Hoaglin (1993), "Volume 16: How to Detect and
        Handle Outliers", The ASQC Basic References in Quality Control:
        Statistical Techniques, Edward F. Mykytka, Ph.D., Editor.
    """
    if len(points.shape) == 1:
        points = points[:, None]
    median = np.median(points, axis=0)
    diff = np.sum((points - median)**2, axis=-1)
    diff = np.sqrt(diff)
    med_abs_deviation = np.median(diff)

    modified_z_score = 0.6745 * diff / med_abs_deviation

    return modified_z_score > thresh


if __name__ == "__main__":
    left_data = {}
    right_data = {}
    file = open("notes.md")
    for line in file:
        split_time = line.split("->")
        if split_time.__len__() > 1:
            no_timestamp = split_time[1]
            data_split = no_timestamp.split(":")
            time_str = split_time[0].strip()
            try:
                time = datetime.strptime(time_str, "%H:%M:%S.%f")
                if data_split.__len__() > 1:
                    data = data_split[1]
                    left = data_split[0].__contains__("left")
                    right = data_split[0].__contains__("right")
                    if left or right:
                        if data.__contains__("."):
                            parsed = 0.0
                            try:
                                parsed = float(data)
                                if left and parsed < 500:
                                    left_data[time_str] = parsed
                                if right and parsed < 500:
                                    right_data[time_str] = parsed
                            except:
                                pass
            except:
                pass
    combined_data = {}
    for key, value in right_data.items():
        if not combined_data.__contains__(key):
            combined_data[key] = {}
        combined_data[key]["right"] = value
    for key, value in left_data.items():
        if not combined_data.__contains__(key):
            combined_data[key] = {}
        combined_data[key]["left"] = value

    times = []
    left_ = []
    right_ = []
    for key, values in combined_data.items():
        times.append(key)
        if values.__contains__("left"):
            left_.append(values["left"])
        else:
            if len(left_) <= 0:
                left_.append(0)
            else:
                left_.append(left_[-1])
        if values.__contains__("right"):
            right_.append(values["right"])
        else:
            if len(right_) <= 0:
                right_.append(0)
            else:
                right_.append(right_[-1])
    print(times.__sizeof__())
    print(left_.__sizeof__())
    print(right_.__sizeof__())

    plt.figure()
    plt.plot(times[:9000], left_[:9000], label="left rpm")
    plt.plot(times[:9000], right_[:9000], label="right rpm")

    plt.xlabel("time")
    plt.ylabel("rpm")
    plt.title("Front RPM data")
    plt.legend()

    plt.show()
