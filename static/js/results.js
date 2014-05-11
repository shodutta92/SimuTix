testResults = []; //List of all simulation results
selectedResult = undefined; //result currently being shown
loadingAnimation = undefined; //loading animation, here so it can be stopped
graphNode = d3.select('#graph svg');

//Get graph data and display it
$(window).load(function() {
    $('#graph').hide();
    generateLoadingAnimation();
    $.get('/fetchResults', function(data) {
        testResults = data.data;
        for (var i = 0; i < testResults.length; i++) {
            testResults[i] = JSON.parse(testResults[i])
        }
        loadingAnimation.stop();
        $('#loading').remove();
        $('#graph').show();
        setupPage();
    });
});
$('.backArrow').click(function() {
    window.location.href = '/';
});

//Load graph and sidebar once data is received
function setupPage() {
    selectedResult = testResults[testResults.length - 1];
    chart = [];
    var graphData = updateGraphData('Time', selectedResult['traceHeaders'].sort());
    initializeGraph(graphData);
    $('#econServers').html(selectedResult['inputs']['econServers'].join('&nbsp&nbsp'));
    $('#fcServers').html(selectedResult['inputs']['fcServers'].join('&nbsp&nbsp'));

    //Create past runs for sidebar in descending order of time
    for (var i = testResults.length - 1; i >= 0; i--) {
        var result = testResults[i];
        var econServers = result['inputs']['econServers'];
        var fcServers = result['inputs']['fcServers']
        var missfl = result['statistics']['MISSFL']['maximum']
        var maxQueue = Math.max(result['statistics']['QUEUE[0]']['maximum'], 
            result['statistics']['QUEUE[1]']['maximum']);
        var isActive = (i == testResults.length - 1);
        var optimized = result['optimized']
        createPastRun(i, econServers, fcServers, missfl, maxQueue, optimized, isActive);
    }
}

function initializeGraph(graphData) {
    chart = nv.models.lineChart().margin({
        left: 75,
        right: 40
    })
    .useInteractiveGuideline(true)
    .transitionDuration(350)
    .showLegend(true)
    .showYAxis(true)
    .showXAxis(true)
    ;
    chart.xAxis
    .axisLabel('Time').tickFormat(d3.format(',r'));
    chart.yAxis
    .axisLabel('Data').tickFormat(d3.format('.02f'));
    graphNode.datum(graphData).transition(300).call(chart);
    nv.utils.windowResize(function() {
        chart.update()
    });
    return chart;
}

function updateGraphData(xaxis, yaxis) {
    var colors = ['#ff8800', '#eeff38', '#0af50a', '#03c4ff', '#ff1232', '#ff0aff'];
    var data = []
    var padding = "   ";
    var labels = []
    var label = '';
    for (var i = 0; i < yaxis.length; i++) {
        //Skip irrelevant trace variables
        if (yaxis[i] == 'Count' || yaxis[i] == 'Time' || yaxis[i] == 'Event') {
            continue;
        }

        //Change graph labels into more useful names
        label = yaxis[i].replace('QUEUE[0]', 'Queue (E)')
        label = label.replace('QUEUE[1]', 'Queue (FC)')
        label = label.replace('SERVER[0]', 'Servers (E)')
        label = label.replace('SERVER[1]', 'Servers (FC)')
        label = label.replace('MISSFLFC', 'Missed Flights (FC)')
        label = label.replace('MISSFL', 'Missed Flights (E)')
        var selectedEvents = selectedResult.events;
        var yaxisName = yaxis[i];
        var line = {
            values: [
                []
            ],
            key: label + padding,
            color: colors[data.length]
        };
        for (var j = 0; j < selectedEvents.length; j++) {
            var maxQueue = selectedResult['statistics']['QUEUE[0]']['maximum']
            if (j % 200 == 0 || selectedEvents[j][yaxis[i]] == maxQueue) {
                line.values.push({
                    x: selectedEvents[j][xaxis],
                    y: selectedEvents[j][yaxisName]
                });
            }
        }
        data.push(line);
    }
    return data;
}

function generateLoadingAnimation() {
    var loader = {
        width: 300,
        height: 300,
        stepsPerFrame: 3,
        trailLength: 0.9,
        pointDistance: 0.01,
        fps: 20,
        fillColor: '#66D9EF',
        path: [
            ['arc', 150, 150, 100, 0, 360]
        ],
        step: function(point, index, frame) {
            var sizeMultiplier = 7;
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
    span.innerHTML = 'Loading...';
    d.appendChild(span);
    container.appendChild(d);
    loadingAnimation.play();
}

//Creates past runs for use in sidebar, redraw graph on click
function createPastRun(id, econServers, fcServers, missfl, maxQueue, optimized, isActive) {
    $('<div class="results-pastRun" id=' + id + 'pastRun><p>Economy:<br>' 
        + econServers.join(',') 
        + '</p><p>First Class:<br>' 
        + fcServers.join(',') + '</p><p>Missed Flights: ' 
        + missfl + '</p><p>Max Queue: ' + maxQueue + '</p><p>Optimized: ' 
        + optimized + '</p></div>').appendTo('#pastRuns').click(function(event) {
        selectedResult = testResults[id]
        var graphData2 = updateGraphData('Time', selectedResult['traceHeaders'].sort());
        $('#econServers').html(selectedResult['inputs']['econServers'].join('&nbsp&nbsp'));
        $('#fcServers').html(selectedResult['inputs']['fcServers'].join('&nbsp&nbsp'));
        graphNode.datum(graphData2).transition(300).call(chart);
        $('.results-pastRun').removeClass('active');
        $(this).addClass('active');
    }).addClass(isActive ? 'active' : '');
}