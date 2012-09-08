import re

class SigmaOutput:
	def __init__(self):
		self.headers = dict()
		self.traceColumns = None
		self.statistics = None
	
	def setTraceHeaders(self, headers):
		for index, header in enumerate(headers):
			self.headers[header] = index
	
	def setTraceColumns(self, traceColumns):
		self.traceColumns = traceColumns
	
	def setStatistics(self, statistics):
		self.statistics = statistics
	
	def getColumn(self, name):
		index = self.headers[name]
		return self.traceColumns[index]

def parse(outputFile):
	sigmaOutput = SigmaOutput()
	separator = re.compile('\s+')
	outputHandle = open(outputFile, 'r')
	parseSummary = False

	headers = outputHandle.next()
	headers = separator.split(headers.strip())
	sigmaOutput.setTraceHeaders(headers)

	#Handle trace columns
	traceColumns = []
	for i in headers:
		traceColumns.append([])

	for line in outputHandle:
		row = separator.split(line.strip())
		if 'SUMMARY' in row[0]:
			parseSummary = True
			break

		for i in range(0, len(row)):
			if i != 1:
				row[i] = float(row[i])
			traceColumns[i].append(row[i])
	sigmaOutput.setTraceColumns(traceColumns)

	#Handle summary statistics
	if parseSummary:
		variableName = None
		statistics = dict()
		qualifier = re.compile("(\D+)\s*=\s*([\d\.e\+\-]+)")

		for line in outputHandle:
			if ":" in line:
				variableName = line[:-1].strip()
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
							statistics[variableName]['average']['event'] = value
					elif "Var" in match[0]:
						if "Time" in match[0]:
							statistics[variableName]['variance']['time'] = value
						else:
							statistics[variableName]['variance']['event'] = value
					else:
						statistics[variableName][match[0].strip().lower()] = value
		sigmaOutput.setStatistics(statistics)
		
	outputHandle.close()
	return sigmaOutput