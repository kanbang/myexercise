#!/usr/bin/evn python3
import sys
import os
import time
import urllib.request, urllib.parse, urllib.error
from threading import Thread

local_proxies = {'http': 'http://131.139.58.200:8080'}


class AxelPython(Thread, urllib.request.FancyURLopener):
    def __init__(self, threadName, url, filename, ranges=0, proxies={}):
        Thread.__init__(self, name=threadName)
        urllib.request.FancyURLopener.__init__(self, proxies)
        self.name = threadName
        self.url = url
        self.filename = filename
        self.ranges = ranges
        self.downloaded = 0

    def run(self):

        try:
            self.downloaded = os.path.getsize(self.filename)
        except OSError:

            self.downloaded = 0

        self.startPoint = self.ranges[0] + self.downloaded

        if self.startPoint >= self.ranges[1]:
            print('Part %s has been downloaded over.' % self.filename)
            return

        self.oneTimeSize = 16384 #16kByte/time
        #print('task %s will download from %d to %d' % (self.name, self.startPoint, self.ranges[1]))

        self.addheader("Range", "bytes=%d-%d" % (self.startPoint, self.ranges[1]))
        self.addheader("Connection", "keep-alive")
        self.urlHandler = self.open(self.url)

        data = self.urlHandler.read(self.oneTimeSize)

        while data:
            fileHandler = open(self.filename, 'ab+')
            fileHandler.write(data)
            fileHandler.close()

            self.downloaded += len(data)
            data = self.urlHandler.read(self.oneTimeSize)


class Download:
    def __init__(self):
        self.complete = False
        self.process = None

    def getUrlFileSize(self, url, proxies={}):
        urlHandler = urllib.request.urlopen(url)
        return int(urlHandler.info()['Content-Length'])


    def splitBlocks(self, totalSize, blockNumber):
        blockSize = totalSize / blockNumber
        ranges = []
        for i in range(0, blockNumber - 1):
            ranges.append((i * blockSize, i * blockSize + blockSize - 1))
        ranges.append(( blockSize * (blockNumber - 1), totalSize - 1 ))

        return ranges


    def isLive(self, tasks):
        for task in tasks:
            if task.isAlive():
                return True
        return False


    def downLoad(self, url, output, blocks=6, proxies=local_proxies):
        size = self.getUrlFileSize(url, proxies)
        ranges = self.splitBlocks(size, blocks)

        threadName = ["thread_%d" % i for i in range(0, blocks)]
        filename = ["tmpFile_%d" % i for i in range(0, blocks)]

        tasks = []
        for i in range(0, blocks):
            task = AxelPython(threadName[i], url, filename[i], ranges[i])
            task.setDaemon(True)
            task.start()
            tasks.append(task)

        time.sleep(2)
        while self.isLive(tasks):
            downloaded = sum([task.downloaded for task in tasks])
            process = downloaded / float(size) * 100
            show = '\rfileSize:%d Downloaded:%d Completed:%.2f%%' % (size, downloaded, process)
            self.process = process
            sys.stdout.write(show)
            sys.stdout.flush()
            time.sleep(0.01)

        self.complete = True
        fileHandler = open(output, 'wb+')
        for i in filename:
            f = open(i, 'rb')
            fileHandler.write(f.read())
            f.close()
            try:
                os.remove(i)
                pass
            except:
                pass

        fileHandler.close()


if __name__ == '__main__':
    url = "http://ftp.ticklers.org/releases.ubuntu.org/releases//precise/ubuntu-12.04.2-server-i386.iso"
    output = 'ubuntu.iso'
    d = Download()
    d.downLoad(url, output, blocks=20, proxies={})
