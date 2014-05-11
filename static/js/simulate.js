flightList = [];
simulationResults = {};
$('#timeAlert').hide();
$('#serverTableDiv').hide();
$('#simulateProgress').hide();
$('#maxQueueDiv').hide();
$('#uploadCSV').click(function() {
    $('#hiddenUploadCSV').trigger('click');
    this.blur();
});
$('#hiddenUploadCSV').on('change', function() {
    uploadFile();
});
$('#simulateButton').click(function() {
    this.blur();
    generateLoadingAnimation();
    var econServers = [];
    var fcServers = [];
    for (var i = 0; i < 6; i++) {
        econServers.push(parseInt($('#es' + i).val()));
        fcServers.push(parseInt($('#fc' + i).val()));
    }
    var optimize = $('.simulate-optimizeButtons .active input').attr('id') == 'optimize'
    var maxEcon = parseInt($('#maxEcon').val());
    var maxFC = parseInt($('#maxFC').val());
    if (optimize) {
        $('#simulateProgress').show();
    }
    $.ajax({
        url: '/simulate',
        type: "POST",
        data: JSON.stringify({
            'flights': flightList,
            'econServers': econServers,
            'fcServers': fcServers,
            'fcPercent': 0.1,
            'runTime': 1440, //Should make this a changeable option
            'optimize': optimize,
            'maxEcon': maxEcon,
            'maxFC': maxFC
        }),
        contentType: "application/json; charset=utf-8",
        success: function(response) {
            window.location.href = '/results';
        }
    });
});
$('#flightsTableButton').click(function() {
    $('#serverTableDiv').hide();
    $('#flightTableDiv').show();
    this.blur();
});
$('#serversTableButton').click(function() {
    $('#flightTableDiv').hide();
    $('#serverTableDiv').show();
    this.blur();
});
$('#addFlightButton').click(function() {
    this.blur();
    var flightNumber = $('#flightNumberInput').val();
    var departureTime = $('#departureTimeInput').val();
    var prettyDepartureTime = Number(departureTime);
    if (prettyDepartureTime) {
        prettyDepartureTime = moment('00:00:00', 'hhmmss').add('m', departureTime).format("hh:mm a");
    } else {
        var time = moment(departureTime, ['hh:mm a', 'h:mm a', 'hh:mma', 'h:mma'], true);
        if (time.isValid()) {
            prettyDepartureTime = time.format("hh:mm a");
            departureTime = Math.floor(time.diff(moment('00:00:00', 'hhmmss')) / 1000 / 60);
            $('#timeAlert').hide();
        } else {
            $('#timeAlert').show();
            return;
        }
    }
    var numberPassengers = $('#numberPassengers').val();
    flightList.push([flightNumber, departureTime, numberPassengers]);
    if (flightNumber && departureTime && numberPassengers) {
        var string = '<tr><td>' + flightNumber + '</td><td>' + prettyDepartureTime + '</td><td>';
        string += numberPassengers + '</td></tr>'
        $('#flightTable tr:last').after(string);
    }
    $('#flightNumberInput').val('');
    $('#departureTimeInput').val('');
    $('#numberPassengers').val('');
});
//Show/hide max queue buttons for optimize option
$("#optimize").change(function() {
    $('#maxQueueDiv').show();
});
$('#simulate').change(function() {
    $('#maxQueueDiv').hide();
});
$('.backArrow').click(function() {
    window.location.href = '/';
});
var ws = new WebSocket("ws://" + window.location.host + "/websocket");
ws.onopen = function() {
    // ws.send("Hello, world");
};
ws.onmessage = function(evt) {
    var parsedData = JSON.parse(evt.data)
    var i = 0;
    for (i = 0; i < parsedData['econQueues'].length; i++) {
        $('#econQueue' + i).text(parsedData['econQueues'][i]);
        $('#econServers' + i).text(parsedData['econServers'][i]);
        $('#fcQueue' + i).text(parsedData['fcQueues'][i]);
        $('#fcServers' + i).text(parsedData['fcServers'][i]);
    }
};

function uploadFile() {
    var input = $('#hiddenUploadCSV')[0];
    if (input.files[0]) {
        var file = input.files[0];
        var reader = new FileReader();
        reader.onload = function() {
            var result = reader.result;
            var rows = result.split("\n");
            var columns = [];
            for (i = 0; i < rows.length; i++) {
                columns = rows[i].split(",");
                var add = "<tr>"
                row = [];
                for (j = 0; j < columns.length; j++) {
                    row[j] = columns[j];
                    if (j == 1) {
                        //add to table
                        row[j] = parseInt(columns[j]);
                        var date = moment("00:00:00", "hhmmss");
                        date.add('m', parseInt(columns[j]));
                        columns[j] = date.format("hh:mm a");
                    }
                    add = add + "<td>" + columns[j] + "</td>"
                }
                flightList.push(row);
                add = add + "</tr>";
                $('#flightTable tr:last').after(add);
            }
        }
        reader.readAsText(file);
    }
}

function generateLoadingAnimation() {
    var loader = {
        width: 100,
        height: 100,
        stepsPerFrame: 3,
        trailLength: 0.9,
        pointDistance: 0.01,
        fps: 20,
        fillColor: '#66D9EF',
        path: [
            ['arc', 50, 50, 30, 0, 360]
        ],
        step: function(point, index, frame) {
            var sizeMultiplier = 5;

            this._.beginPath();
            this._.moveTo(point.x, point.y);
            this._.arc(point.x, point.y, index * sizeMultiplier, 0, Math.PI * 2, false);
            this._.closePath();
            this._.fill();
        }
    }
    var d, a, container = document.getElementById('loading');
    container.innerHTML = '';
    d = document.createElement('div');
    d.className = 'l';
    loadingAnimation = new Sonic(loader);
    d.appendChild(loadingAnimation.canvas);
    var span = document.createElement('span');
    span.className = 'results-loadingText';
    span.innerHTML = 'Running...';
    d.appendChild(span);
    container.appendChild(d);
    loadingAnimation.play();
}