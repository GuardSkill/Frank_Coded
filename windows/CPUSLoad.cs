using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Threading;
using System.Collections;

namespace CPUSLoad
{

    class Program
    {
        static void Main(string[] args)
        {

            PerformanceCounter[] CPUCounters =
       new PerformanceCounter[Environment.ProcessorCount];
            for (int i = 0; i < CPUCounters.Length; i++)
            {
                CPUCounters[i] = new PerformanceCounter("Processor", "% Processor Time", i.ToString());
            }
            while (true)
            {
                for (int i = 0; i < CPUCounters.Length; i++)
                {
                    CPUCounters[i].NextValue();           //first value invalid
                }
                System.Threading.Thread.Sleep(1000);
                for (int i = 0; i < CPUCounters.Length; i++)
                {
                    Console.Write("CPU{0}利用率 :{1:f}%    ", i, CPUCounters[i].NextValue());
                }
                Console.WriteLine();
            }
        }
    }
}


