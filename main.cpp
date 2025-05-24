#include <math.h>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <chrono>
#include <random>
#include <ctime>
#include <fstream>
#include <ranges>
#include <functional>

class StringGenerator {
public:
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";
    std::mt19937 rng;
    StringGenerator() { rng.seed(std::time(nullptr)); }

    std::string RandomString() {
        std::uniform_int_distribution<size_t> dist_alphabet(0, alphabet.size() - 1);
        std::uniform_int_distribution<size_t> dist_length(10, 200);
        std::string result;
        const size_t len = dist_length(rng);
        result.reserve(len);
        for (size_t i = 0; i < len; ++i) {
            result += alphabet[dist_alphabet(rng)];
        }
        return result;
    }

    std::vector<std::string> RandomArr(const int type = 0) {
        if (type < 0 || type > 2) {
            throw std::invalid_argument("Invalid type");
        }
        constexpr int kCount = 3000;
        std::vector<std::string> result;
        result.reserve(kCount);
        for (int i = 0; i < kCount; ++i) {
            result.push_back(RandomString());
        }
        if (type == 1) {
            std::ranges::sort(result, std::greater<>());
        } else if (type == 2) {
            std::ranges::sort(result);
            std::uniform_int_distribution<size_t> d(0, kCount - 2);
            std::uniform_int_distribution<size_t> s(3, 7);
            const size_t swaps = s(rng);
            for (size_t i = 0; i < swaps; ++i) {
                const size_t idx = d(rng);
                std::swap(result[idx], result[idx + 1]);
            }
        }
        return result;
    }
};

class StringSortTester {
public:
    static size_t char_comparisons;
    static void ResetCharComps() { char_comparisons = 0; }
    static size_t GetCharComps() { return char_comparisons; }

    static bool LcpCompare(const std::string &a, const std::string &b) {
        const int na = static_cast<int>(a.size());
        const int nb = static_cast<int>(b.size());
        int i = 0;
        while (i < na && i < nb && a[i] == b[i]) {
            ++char_comparisons;
            ++i;
        }
        if (i == na && i == nb) {
            return false;
        }
        if (i == na) {
            return true;
        }
        if (i == nb) {
            return false;
        }
        return a[i] < b[i];
    }

    static void QuickSortClassic(std::vector<std::string> &a, int low, int high) {
        if (low >= high) {
            return;
        }
        const std::string pivot = a[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (LcpCompare(a[j], pivot)) {
                ++i;
                std::swap(a[i], a[j]);
            }
        }
        std::swap(a[i + 1], a[high]);
        const int m = i + 1;
        QuickSortClassic(a, low, m - 1);
        QuickSortClassic(a, m + 1, high);
    }

    static void MergeSortClassic(std::vector<std::string> &a, const int left, const int right,
                                 std::vector<std::string> &tmp) {
        if (left >= right) {
            return;
        }
        const int mid = left + (right - left) / 2;
        MergeSortClassic(a, left, mid, tmp);
        MergeSortClassic(a, mid + 1, right, tmp);
        int i = left;
        int j = mid + 1;
        int k = left;
        while (i <= mid && j <= right) {
            if (LcpCompare(a[i], a[j])) {
                tmp[k++] = std::move(a[i++]);
            } else {
                tmp[k++] = std::move(a[j++]);
            }
        }
        while (i <= mid) {
            tmp[k++] = std::move(a[i++]);
        }
        while (j <= right) {
            tmp[k++] = std::move(a[j++]);
        }
        for (int p = left; p <= right; ++p) {
            a[p] = std::move(tmp[p]);
        }
    }

    static void MergeSort(std::vector<std::string> &arr, const int l, const int r, std::vector<std::string> &tmp) {
        if (l >= r) {
            return;
        }
        const int m = l + (r - l) / 2;
        MergeSort(arr, l, m, tmp);
        MergeSort(arr, m + 1, r, tmp);
        int i = l;
        int j = m + 1;
        int k = l;
        while (i <= m && j <= r) {
            ++char_comparisons;
            if (LcpCompare(arr[i], arr[j])) {
                tmp[k++] = std::move(arr[i++]);
            } else {
                tmp[k++] = std::move(arr[j++]);
            }
        }
        while (i <= m) {
            tmp[k++] = std::move(arr[i++]);
        }
        while (j <= r) {
            tmp[k++] = std::move(arr[j++]);
        }
        for (int p = l; p <= r; ++p) {
            arr[p] = std::move(tmp[p]);
        }
    }

    static int CharAt(const std::string &s, int d) {
        return d < static_cast<int>(s.size()) ? static_cast<unsigned char>(s[d]) : -1;
    }

    static void QuickSort(std::vector<std::string> &a, const int lo, const int hi, const int d) {
        if (lo >= hi) {
            return;
        }
        const int v = CharAt(a[hi], d);

        int lt = lo;
        int i = lo;
        int gt = hi - 1;
        while (i <= gt) {
            const int t = CharAt(a[i], d);
            ++char_comparisons;
            if (t < v) {
                std::swap(a[lt++], a[i++]);
            } else {
                ++char_comparisons;
                if (t > v) {
                    std::swap(a[i], a[gt--]);
                } else {
                    ++i;
                }
            }
        }
        std::swap(a[i], a[hi]);
        QuickSort(a, lo, lt - 1, d);
        if (v >= 0) {
            QuickSort(a, lt, i, d + 1);
        }
        QuickSort(a, i + 1, hi, d);
    }

    static void MsdRadixSort(std::vector<std::string> &a, std::vector<std::string> &aux, const int lo, const int hi,
                             const int d) {
        if (lo >= hi) {
            return;
        }
        constexpr int kR = 256;
        std::array<int, kR + 2> count{};
        for (int i = lo; i <= hi; ++i) {
            const int c = CharAt(a[i], d) + 1;
            ++count[c + 1];
        }
        for (int r = 0; r <= kR; ++r) {
            count[r + 1] += count[r];
        }
        auto start = count;
        for (int i = lo; i <= hi; ++i) {
            const int c = CharAt(a[i], d) + 1;
            aux[start[c]++] = std::move(a[i]);
        }
        for (int i = lo; i <= hi; ++i) {
            a[i] = std::move(aux[i - lo]);
        }
        for (int r = 0; r <= kR; ++r) {
            const int sub_lo = lo + count[r];
            if (const int sub_hi = lo + count[r + 1] - 1; sub_lo <= sub_hi) {
                MsdRadixSort(a, aux, sub_lo, sub_hi, d + 1);
            }
        }
    }

    static void MsdRadixQuickSort(std::vector<std::string> &a, std::vector<std::string> &aux, const int lo,
                                  const int hi, const int d) {
        if (lo >= hi) {
            return;
        }
        constexpr int kR = 256;
        if (const int n = hi - lo + 1; n < 74) {
            QuickSort(a, lo, hi, d);
            return;
        }
        std::array<int, kR + 2> count{};
        for (int i = lo; i <= hi; ++i) {
            const int c = (d < static_cast<int>(a[i].size())) ? static_cast<unsigned char>(a[i][d]) + 1 : 0;
            ++count[c + 1];
        }
        for (int r = 0; r <= kR; ++r) {
            count[r + 1] += count[r];
        }
        auto start = count;
        for (int i = lo; i <= hi; ++i) {
            const int c = (d < static_cast<int>(a[i].size())) ? static_cast<unsigned char>(a[i][d]) + 1 : 0;
            aux[start[c]++] = std::move(a[i]);
        }
        for (int i = lo; i <= hi; ++i) {
            a[i] = std::move(aux[i - lo]);
        }
        for (int r = 0; r <= kR; ++r) {
            const int sub_lo = lo + count[r];
            if (const int sub_hi = lo + count[r + 1] - 1; sub_lo <= sub_hi) {
                MsdRadixQuickSort(a, aux, sub_lo, sub_hi, d + 1);
            }
        }
    }

    static void TestAlgorithms() {
        StringGenerator gen;
        const std::vector<int> sizes = [] {
            std::vector<int> v;
            for (int x = 100; x <= 3000; x += 100) {
                v.push_back(x);
            }
            return v;
        }();
        const std::vector<std::pair<std::string, int> > types = {
            {"random", 0}, {"reverse", 1}, {"almost", 2}
        };
        enum Algo { CLASSIC_QUICK, CLASSIC_MERGE, STR_QUICK, STR_MERGE, MSD_RADIX, MSD_RADIX_Q };
        const std::vector<Algo> algos = {
            CLASSIC_QUICK, CLASSIC_MERGE, STR_QUICK, STR_MERGE, MSD_RADIX, MSD_RADIX_Q
        };
        auto algo_name = [&](const Algo a)-> std::string {
            switch (a) {
                case CLASSIC_QUICK: return "classic_quick";
                case CLASSIC_MERGE: return "classic_merge";
                case STR_QUICK: return "string_quick";
                case STR_MERGE: return "string_merge";
                case MSD_RADIX: return "msd_radix";
                case MSD_RADIX_Q: return "msd_radix_quick";
            }
            return "";
        };
        for (auto algo: algos) {
            std::ofstream fout_time(algo_name(algo) + "_times.txt");
            std::ofstream fout_comp(algo_name(algo) + "_comps.txt");
            for (int sz: sizes) {
                for (auto [tname,tcode]: types) {
                    for (int run = 1; run <= 10; ++run) {
                        auto base = gen.RandomArr(tcode);
                        std::vector data(base.begin(), base.begin() + sz);
                        ResetCharComps();
                        auto t0 = std::chrono::high_resolution_clock::now();
                        switch (algo) {
                            case CLASSIC_QUICK:
                                QuickSortClassic(data, 0, sz - 1);
                                break;
                            case CLASSIC_MERGE: {
                                std::vector<std::string> tmp(sz);
                                MergeSortClassic(data, 0, sz - 1, tmp);
                                break;
                            }
                            case STR_QUICK:
                                QuickSort(data, 0, sz - 1, 0);
                                break;
                            case STR_MERGE: {
                                std::vector<std::string> tmp(sz);
                                MergeSort(data, 0, sz - 1, tmp);
                                break;
                            }
                            case MSD_RADIX: {
                                std::vector<std::string> aux(sz);
                                MsdRadixSort(data, aux, 0, sz - 1, 0);
                                break;
                            }
                            case MSD_RADIX_Q: {
                                std::vector<std::string> aux(sz);
                                MsdRadixQuickSort(data, aux, 0, sz - 1, 0);
                                break;
                            }
                        }
                        auto t1 = std::chrono::high_resolution_clock::now();
                        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
                        size_t comps = GetCharComps();
                        fout_time << ms << '\n';
                        fout_comp << comps << '\n';
                    }
                }
            }
        }
    }

    static double ComputeAverage(const std::string &filename) {
        std::ifstream in(filename);
        double sum = 0.0;
        double x = NAN;
        size_t count = 0;
        while (in >> x) {
            sum += x;
            ++count;
        }
        return count ? sum / static_cast<double>(count) : 0.0;
    }

    static void TestResults() {
        const std::vector<std::string> algos = {
            "classic_merge",
            "classic_quick",
            "msd_radix",
            "msd_radix_quick",
            "string_merge",
            "string_quick"
        };
        std::cout << std::fixed << std::setprecision(3);
        for (const auto &algo : algos) {
            const std::string times_file = algo + "_times.txt";
            const double avg_time = ComputeAverage(times_file);
            const std::string comps_file = algo + "_comps.txt";
            const double avg_comps = ComputeAverage(comps_file);
            const std::string theory = algo.rfind("msd", 0) == 0 ? "omega(n)" : "omega(nlogn)";
            std::cout << algo << '\n';
            std::cout << "Average time:\t\t" << avg_time  << " ms (theory: " << theory << ")\n";
            std::cout << "Average comparison:\t" << avg_comps << "\n\n";
        }
    }
};

size_t StringSortTester::char_comparisons = 0;

int main() {
    StringSortTester::TestAlgorithms();
    StringSortTester::TestResults();
    return 0;
}
