# from scipy.stats import bernoulli, norm
import csv
import random


class Passenger:

    def __init__(self, passengerNumber, arrivalTime, baggage, ticket, departureTime):
        self.passengerNumber = passengerNumber
        self.arrivalTime = arrivalTime
        self.baggage = baggage
        self.ticket = ticket
        self.departureTime = departureTime


def createDataFile(flightData, fileName):
    #Beta Distribution Parameters
    alpha = 4
    beta = 3
    minTime = 30
    timeWidth = 90

    pBags = 0.8
    pTicket = 0.5

    csvoutput = open(fileName, 'wb')
    writer = csv.writer(csvoutput, delimiter='\t')

    # input: Flight#, Departure Time (minutes after midnight), # Passengers
    # output: Passenger Number, Arrival Time, Baggage, Ticket, Departure Time

    passengerList = []
    passengerNumber = 0

    #create passenger information
    for row in flightData:
        flight = row[0]
        departureTime = int(row[1])
        numberPassengers = int(row[2])
        for i in range(0, numberPassengers):
            arrivalTime = departureTime - (timeWidth * random.betavariate(alpha, beta) + minTime)
            baggage = 1 if random.random() < pBags else 0
            ticket = 1 if random.random() < pTicket else 0
            passengerList.append(
                Passenger(passengerNumber, arrivalTime, baggage, ticket, departureTime))
            passengerNumber += 1

    passengerList = [i for i in passengerList if i.arrivalTime >= 0]
    passengerList.sort(key=lambda x: x.arrivalTime)

    #Write data file
    writer.writerow([len(passengerList)])
    for passenger in passengerList:
        writer.writerow([passenger.passengerNumber, passenger.arrivalTime,
                        passenger.baggage, passenger.ticket, passenger.departureTime])
