import matplotlib.pyplot as plt
import numpy as np

points = np.arange(900)

files = [
    'classic_merge_comps.txt',
    'classic_quick_comps.txt',
    'msd_radix_comps.txt',
    'msd_radix_quick_comps.txt',
    'string_merge_comps.txt',
    'string_quick_comps.txt',

    'classic_merge_times.txt',
    'classic_quick_times.txt',
    'msd_radix_times.txt',
    'msd_radix_quick_times.txt',
    'string_merge_times.txt',
    'string_quick_times.txt'
]

data = {}
for filename in files:
    with open(filename, "r") as file:
        data[filename] = sorted([float(line.strip()) for line in file])

for i in range(0, len(files), 6):
    plt.figure(figsize=(10, 6))
    plt.plot(points, data[files[i]], label="Merge стандартный", color="red")
    plt.plot(points, data[files[i + 1]], label="Quick стандартный", color="orange")
    plt.plot(points, data[files[i + 2]], label="MSD RADIX SORT без переключения", color="yellow")
    plt.plot(points, data[files[i + 3]], label="MSD RADIX SORT с переключением", color="green")
    plt.plot(points, data[files[i + 4]], label="STRING MERGESORT", color="blue")
    plt.plot(points, data[files[i + 5]], label="STRING QUICKSORT тернарный", color="violet")
    plt.title(['Количество посимвольных сравнений', 'Время работы'][i // 6] + ' (логарифмический график)')
    plt.yscale('log')
    plt.legend()
    plt.grid(True)
    plt.show()
