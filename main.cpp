#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <string>
using namespace std;

#define TRACE_MODE "trace"
#define SHOW_STATS "stats"

vector<vector<char>> timeline;
vector<int> finish_time;
vector<int> turnaround_time;
vector<double> norm_turnaround;
vector<int> wait_time;

int process_count;
int max_time;

vector<tuple<string, int, int>> processes;
vector<string> processID_list;

bool parseFile(string file_name)
{
    ifstream infile(file_name);
    if (!infile)
        return false;

    string processID;
    int arrivalTime, serviceTime;

    while (infile >> processID >> arrivalTime >> serviceTime)
    {
        processes.push_back(make_tuple(processID, arrivalTime, serviceTime));
        processID_list.push_back(processID);
    }
    infile.close();
    process_count = processes.size();
    return true;
}

void reset_timeline()
{
    timeline = vector<vector<char>>(max_time, vector<char>(process_count, ' '));
    finish_time = vector<int>(process_count, 0);
    turnaround_time = vector<int>(process_count, 0);
    norm_turnaround = vector<double>(process_count, 0.0);
    wait_time = vector<int>(process_count, 0);
}

void printResults()
{
    cout << "Timeline:\n";
    for (int i = 0; i < process_count; i++)
        cout << get<0>(processes[i]) << "\t";
    cout << "\n";
    for (int i = 0; i < max_time; i++)
    {
        for (int j = 0; j < process_count; j++)
            cout << timeline[i][j] << "\t";
        cout << "\n";
    }
}

void printTrace()
{
    cout << "Trace:\n";
    for (int i = 0; i < max_time; i++)
    {
        cout << i + 1 << ":\t";
        for (int j = 0; j < process_count; j++)
            cout << timeline[i][j] << "\t";
        cout << "\n";
    }
}

void printStats()
{
    double total_turnaround = 0;
    double total_norm_turnaround = 0;
    for (int i = 0; i < process_count; i++)
    {
        total_turnaround += turnaround_time[i];
        total_norm_turnaround += norm_turnaround[i];
    }

    cout << "Stats:\n";
    cout << "Average Turnaround Time: " << total_turnaround / process_count << "\n";
    cout << "Average Normalized Turnaround Time: " << total_norm_turnaround / process_count << "\n";
}

void markWaitTime()
{
    for (int i = 0; i < process_count; i++)
        wait_time[i] = turnaround_time[i] - get<2>(processes[i]);
}

int getProcessArrivalTime(tuple<string, int, int> &proc)
{
    return get<1>(proc);
}

int getProcessServiceTime(tuple<string, int, int> &proc)
{
    return get<2>(proc);
}

bool compareArrivalTime(tuple<string, int, int> &a, tuple<string, int, int> &b)
{
    return get<1>(a) < get<1>(b);
}

bool compareServiceTime(tuple<string, int, int> &a, tuple<string, int, int> &b)
{
    return get<2>(a) < get<2>(b);
}

bool compareResponseRatio(tuple<string, double, int> &a, tuple<string, double, int> &b)
{
    return get<1>(a) > get<1>(b);
}

void firstComeFirstServe()
{
    queue<int> readyQueue;
    int currentIndex = 0;
    for (int current_time = 0; current_time < max_time; current_time++)
    {
        while (currentIndex < process_count && getProcessArrivalTime(processes[currentIndex]) <= current_time)
        {
            readyQueue.push(currentIndex);
            currentIndex++;
        }
        if (!readyQueue.empty())
        {
            int procIndex = readyQueue.front();
            readyQueue.pop();
            int arrivalTime = getProcessArrivalTime(processes[procIndex]);
            int serviceTime = getProcessServiceTime(processes[procIndex]);
            for (int tempTime = arrivalTime; tempTime < current_time; tempTime++)
                timeline[tempTime][procIndex] = '.';
            for (int tempTime = current_time; tempTime < current_time + serviceTime; tempTime++)
                timeline[tempTime][procIndex] = '*';
            finish_time[procIndex] = (current_time + serviceTime);
            turnaround_time[procIndex] = (finish_time[procIndex] - arrivalTime);
            norm_turnaround[procIndex] = (turnaround_time[procIndex] * 1.0 / serviceTime);
            current_time += serviceTime - 1;
        }
    }
    markWaitTime();
}

void roundRobin(int timeQuantum)
{
    queue<pair<int, int>> readyQueue; // Pair: (processIndex, remainingTime)
    int currentIndex = 0;
    for (int current_time = 0; current_time < max_time; current_time++)
    {
        while (currentIndex < process_count && getProcessArrivalTime(processes[currentIndex]) <= current_time)
        {
            readyQueue.push({currentIndex, getProcessServiceTime(processes[currentIndex])});
            currentIndex++;
        }
        if (!readyQueue.empty())
        {
            auto front = readyQueue.front();
            readyQueue.pop();
            int procIndex = front.first;
            int remainingTime = front.second;
            int arrivalTime = getProcessArrivalTime(processes[procIndex]);
            for (int tempTime = current_time; tempTime < current_time + timeQuantum && tempTime < max_time; tempTime++)
            {
                if (remainingTime > 0)
                {
                    timeline[tempTime][procIndex] = '*';
                    remainingTime--;
                    if (remainingTime == 0)
                    {
                        finish_time[procIndex] = (tempTime + 1);
                        turnaround_time[procIndex] = (finish_time[procIndex] - arrivalTime);
                        norm_turnaround[procIndex] = (turnaround_time[procIndex] * 1.0 / getProcessServiceTime(processes[procIndex]));
                    }
                }
            }
            if (remainingTime > 0)
                readyQueue.push({procIndex, remainingTime});
        }
    }
    markWaitTime();
}

void shortestProcessNext()
{
    vector<int> readyQueue;
    int currentIndex = 0;
    for (int current_time = 0; current_time < max_time; current_time++)
    {
        while (currentIndex < process_count && getProcessArrivalTime(processes[currentIndex]) <= current_time)
        {
            readyQueue.push_back(currentIndex);
            currentIndex++;
        }
        if (!readyQueue.empty())
        {
            sort(readyQueue.begin(), readyQueue.end(), [&](int a, int b) { return getProcessServiceTime(processes[a]) < getProcessServiceTime(processes[b]); });
            int procIndex = readyQueue.front();
            readyQueue.erase(readyQueue.begin());
            int arrivalTime = getProcessArrivalTime(processes[procIndex]);
            int serviceTime = getProcessServiceTime(processes[procIndex]);
            for (int tempTime = arrivalTime; tempTime < current_time; tempTime++)
                timeline[tempTime][procIndex] = '.';
            for (int tempTime = current_time; tempTime < current_time + serviceTime; tempTime++)
                timeline[tempTime][procIndex] = '*';
            finish_time[procIndex] = (current_time + serviceTime);
            turnaround_time[procIndex] = (finish_time[procIndex] - arrivalTime);
            norm_turnaround[procIndex] = (turnaround_time[procIndex] * 1.0 / serviceTime);
            current_time += serviceTime - 1;
        }
    }
    markWaitTime();
}

void shortestRemainingTime()
{
    vector<tuple<string, int, int>> ready_processes; // Tuple: (processID, remainingTime, startTime)
    int currentIndex = 0;
    for (int current_time = 0; current_time < max_time; current_time++)
    {
        while (currentIndex < process_count && getProcessArrivalTime(processes[currentIndex]) <= current_time)
        {
            ready_processes.push_back(make_tuple(get<0>(processes[currentIndex]), getProcessServiceTime(processes[currentIndex]), current_time));
            currentIndex++;
        }
        if (!ready_processes.empty())
        {
            sort(ready_processes.begin(), ready_processes.end(), [&](tuple<string, int, int> &a, tuple<string, int, int> &b) { return get<1>(a) < get<1>(b); });
            auto &proc = ready_processes.front();
            int procIndex = distance(processID_list.begin(), find(processID_list.begin(), processID_list.end(), get<0>(proc)));
            int arrivalTime = getProcessArrivalTime(processes[procIndex]);
            int serviceTime = getProcessServiceTime(processes[procIndex]);
            get<1>(proc)--;
            timeline[current_time][procIndex] = '*';
            if (get<1>(proc) == 0)
            {
                finish_time[procIndex] = (current_time + 1);
                turnaround_time[procIndex] = (finish_time[procIndex] - arrivalTime);
                norm_turnaround[procIndex] = (turnaround_time[procIndex] * 1.0 / serviceTime);
                ready_processes.erase(ready_processes.begin());
            }
        }
    }
    markWaitTime();
}

void highestResponseRatioNext()
{
    vector<tuple<string, double, int>> ready_processes; // Tuple: (processID, responseRatio, startTime)
    int currentIndex = 0;
    for (int current_time = 0; current_time < max_time; current_time++)
    {
        while (currentIndex < process_count && getProcessArrivalTime(processes[currentIndex]) <= current_time)
        {
            ready_processes.push_back(make_tuple(get<0>(processes[currentIndex]), 0.0, current_time));
            currentIndex++;
        }
        if (!ready_processes.empty())
        {
            for (auto &proc : ready_processes)
            {
                int procIndex = distance(processID_list.begin(), find(processID_list.begin(), processID_list.end(), get<0>(proc)));
                int arrivalTime = getProcessArrivalTime(processes[procIndex]);
                int serviceTime = getProcessServiceTime(processes[procIndex]);
                get<1>(proc) = (current_time - arrivalTime + serviceTime) * 1.0 / serviceTime;
            }
            sort(ready_processes.begin(), ready_processes.end(), compareResponseRatio);
            auto &proc = ready_processes.front();
            int procIndex = distance(processID_list.begin(), find(processID_list.begin(), processID_list.end(), get<0>(proc)));
            int arrivalTime = getProcessArrivalTime(processes[procIndex]);
            int serviceTime = getProcessServiceTime(processes[procIndex]);
            get<2>(proc)--;
            timeline[current_time][procIndex] = '*';
            if (get<2>(proc) == 0)
            {
                finish_time[procIndex] = (current_time + 1);
                turnaround_time[procIndex] = (finish_time[procIndex] - arrivalTime);
                norm_turnaround[procIndex] = (turnaround_time[procIndex] * 1.0 / serviceTime);
                ready_processes.erase(ready_processes.begin());
            }
        }
    }
    markWaitTime();
}

void execute(string algorithm, int timeQuantum = 1)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);
    max_time = 0;
    for (auto &proc : processes)
        max_time += getProcessServiceTime(proc);
    reset_timeline();

    if (algorithm == "FCFS")
        firstComeFirstServe();
    else if (algorithm == "RR")
        roundRobin(timeQuantum);
    else if (algorithm == "SPN")
        shortestProcessNext();
    else if (algorithm == "SRT")
        shortestRemainingTime();
    else if (algorithm == "HRRN")
        highestResponseRatioNext();

    printResults();
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " <input_file> <algorithm> [time_quantum]\n";
        return 1;
    }

    string input_file = argv[1];
    string algorithm = argv[2];
    int timeQuantum = 1;

    if (argc == 4)
        timeQuantum = stoi(argv[3]);

    if (!parseFile(input_file))
    {
        cout << "Error reading file\n";
        return 1;
    }

    execute(algorithm, timeQuantum);

    return 0;
}
