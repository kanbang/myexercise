class Log():
    def __init__(self, file, model):
        self.file = file
        self.mode = model

    def write(self, data):
        self.fileHandle = open(self.file, self.mode)
        self.fileHandle.write(data)

    def close(self):
        self.fileHandle.close()