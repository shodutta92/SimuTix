import re
import json


class SigmaOutput:

    def __init__(self):
        self.statistics = None
        self.events = None
        self.inputs = None
        self.traceHeaders = None
        self.optimized = False

    def toJSON(self):
        data = {}
        data['statistics'] = self.statistics
        data['events'] = self.events
        data['inputs'] = self.inputs
        data['traceHeaders'] = self.traceHeaders
        data['optimized'] = self.optimized
        return json.dumps(data)

    def setEvents(self, events):
        self.events = events

    def setStatistics(self, statistics):
        self.statistics = statistics

    def setInputs(self, inputs):
        self.inputs = inputs

    def setTraceHeaders(self, headers):
        self.traceHeaders = headers

    def setOptimized(self, optimized):
        self.optimized = optimized


def parse(outputFile, time, inputs):
    sigmaOutput = SigmaOutput()
    separator = re.compile('\s+')
    outputHandle = open(outputFile, 'r')
    parseSummary = False

    events = []
    sigmaOutput.setInputs(inputs)

    headers = outputHandle.next()
    headers = separator.split(headers.strip())
    sigmaOutput.setTraceHeaders(headers)

    # Handle trace columns
    traceColumns = []
    for i in headers:
        traceColumns.append([])

    for line in outputHandle:
        row = separator.split(line.strip())
        if 'SUMMARY' in row[0]:
            parseSummary = True
            break

        event = {}
        for i in range(0, len(row)):
            if i != 1:
                row[i] = float(row[i])
            traceColumns[i].append(row[i])
            event[headers[i]] = row[i]
        events.append(event)
    sigmaOutput.setEvents(events)

    # Handle summary statistics
    if parseSummary:
        variableName = None
        statistics = dict()
        qualifier = re.compile("(\D+)\s*=\s*([\d\.e\+\-]+)")

        for line in outputHandle:
            if ":" in line:
                variableName = line.strip()[:-1]
                statistics[variableName] = dict()
                statistics[variableName]['average'] = dict()
                statistics[variableName]['variance'] = dict()
            else:
                matches = qualifier.findall(line.strip())
                for match in matches:
                    value = float(match[1].strip())
                    if "Ave" in match[0]:
                        if "Time" in match[0]:
                            statistics[variableName]['average']['time'] = value
                        else:
                            statistics[variableName][
                                'average']['event'] = value
                    elif "Var" in match[0]:
                        if "Time" in match[0]:
                            statistics[variableName][
                                'variance']['time'] = value
                        else:
                            statistics[variableName][
                                'variance']['event'] = value
                    else:
                        statistics[variableName][
                            match[0].strip().lower()] = value
        sigmaOutput.setStatistics(statistics)

    outputHandle.close()
    return sigmaOutput
