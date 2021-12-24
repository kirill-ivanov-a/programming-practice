﻿using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;
using System.Threading;
using FibersLib;

namespace Fibers.Tests
{
    [TestClass]
    public class PriorityFibersTest
    {
        int numOfProc = 5;
        int numOfFinishedProc;

        [TestInitialize]
        public void Init()
        {
            Thread.Sleep(100);
            ProcessManager.IsPriority = false;
            for (int i = 0; i < numOfProc; i++)
            {
                ProcessManager.AddProcess(new Process());
            }
            ProcessManager.Run();
            numOfFinishedProc = ProcessManager.Processes
                .Count(p => p.Value.Status == ProcessStatus.Finished);
            Thread.Sleep(1000);
            ProcessManager.Dispose();
        }

        [TestMethod]
        public void CorrectFibersFinishing()
        {
            Assert.AreEqual(numOfProc, numOfFinishedProc);
        }
    }
}
