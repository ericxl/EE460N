using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class UnitTest
    {
        const string sourceFilePath = @"C:\depot\EE460N\ee460n-proj1\TestCases";
        const string programPath = @"C:\depot\EE460N\ee460n-proj1\x64\Debug\ee460n-proj1.exe";
        [TestMethod]
        public void Test1()
        {
            var testName = "test1";
            var inputPath = sourceFilePath + @"\" + testName + @".asm";
            var outputPath = sourceFilePath + @"\" + testName + @".obj";
            var expectedPath = sourceFilePath + @"\" + testName + @".txt";
            var startInfo = new ProcessStartInfo();
            startInfo.CreateNoWindow = false;
            startInfo.UseShellExecute = false;
            startInfo.FileName = programPath;
            startInfo.WindowStyle = ProcessWindowStyle.Hidden;
            startInfo.Arguments = inputPath + " " + outputPath;

            var exeProcess = Process.Start(startInfo);
            exeProcess.WaitForExit();

            var sourcesLines = File.ReadAllLines(outputPath);
            var expectedLines = File.ReadAllLines(expectedPath);

            for (var i = 0; i < sourcesLines.Length; i++)
            {
                var line = sourcesLines[i];
                var expected = expectedLines[i];
                Assert.IsTrue(string.Equals(line, expected, StringComparison.OrdinalIgnoreCase));
            }
        }

        [TestMethod]
        public void Test2()
        {
            var testName = "test2";
            var inputPath = sourceFilePath + @"\" + testName + @".asm";
            var outputPath = sourceFilePath + @"\" + testName + @".obj";
            var expectedPath = sourceFilePath + @"\" + testName + @".txt";
            var startInfo = new ProcessStartInfo();
            startInfo.CreateNoWindow = false;
            startInfo.UseShellExecute = false;
            startInfo.FileName = programPath;
            startInfo.WindowStyle = ProcessWindowStyle.Hidden;
            startInfo.Arguments = inputPath + " " + outputPath;

            var exeProcess = Process.Start(startInfo);
            exeProcess.WaitForExit();

            var sourcesLines = File.ReadAllLines(outputPath);
            var expectedLines = File.ReadAllLines(expectedPath);

            for (var i = 0; i < sourcesLines.Length; i++)
            {
                var line = sourcesLines[i];
                var expected = expectedLines[i];
                var mes = "expected is " + expected + "    actual is " + line;
                Assert.IsTrue(string.Equals(line, expected, StringComparison.OrdinalIgnoreCase),mes);
            }
        }
    }
}
