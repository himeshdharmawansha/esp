<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Sensor Dashboard</title>
  <style>
    /* Basic reset */
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f9;
      color: #333;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }

    .main {
      display: flex;
      justify-content: space-between;
      max-width: 1000px;
      width: 100%;
      padding: 20px;
    }

    .leftside {
      flex: 1;
      flex-direction: column;
      padding: 20px;
      display: flex;
      justify-content: center;
      align-items: center;
    }

    .rightside {
      flex: 2;
      display: flex;
      flex-direction: column;
      /* justify-content:; */
    }

    .container {
      background: #ffffff;
      border-radius: 8px;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      padding: 20px;
      margin-bottom: 20px;
      text-align: center;
    }

    h2 {
      font-size: 1.5em;
      margin-bottom: 10px;
      color: #444;
    }

    .value {
      font-size: 2.5em;
      color: #007BFF;
      font-weight: bold;
      margin: 10px 0;
    }

    .status {
      font-size: 1.2em;
      margin-top: 10px;
    }

    .status.safe {
      color: green;
    }

    .status.warning {
      color: orange;
    }

    .status.danger {
      color: red;
    }

    .leftside img {
      max-width: 100%;
      border-radius: 8px;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
    }

    /* Message when bin is full */
    .message {
      color: red;
      font-weight: bold;
      font-size: 1.5em;
      margin-top: 20px;
      display: none;  /* Hide initially */
    }

    /* Responsive design */
    @media (max-width: 768px) {
      .main {
        flex-direction: column;
      }

      .leftside {
        margin-bottom: 20px;
      }
    }
  </style>
</head>
<body>
    <div class="main">
        <section class="leftside">
            <h1 style="font-family: 'Arial', sans-serif; color:#007BFF; font-size: 3em;  border-radius: 10px;">
                Binfinity
              </h1>
                          <img class="container" src="https://img.freepik.com/free-vector/recycle-bin-ecological-sustainability-icon-isolated_24640-134036.jpg?t=st=1739950412~exp=1739954012~hmac=c61e5cb2cfe6f218d929cc0538b69c214270f7e5cf76c76240e04982a08e97a9&w=360" alt="Recycle Bin">
        </section>
        <section class="rightside">
            <div class="container">
                <h2>Bin Filling Status</h2>
                <p>Bin Filling: <span class="value" id="distance">-- %</span></p>
                <p id="status" class="status">Status: --</p>
            </div>
            
            <div class="container">
                <h2>Environmental Data</h2>
                <p>Temperature: <span class="value" id="temp">-- °C</span></p>
                <p>Humidity: <span class="value" id="hum">-- %</span></p>
            </div>

            <div class="container message" id="fullMessage">
                <p >The trash bin is full (90%+).</p>
            </div>
        </section>
    </div>
  <script>
    const maxDistance = 100;  // Max distance (empty bin)
    const minDistance = 10;   // Min distance (full bin)

    const data = {
        temperature: 40,
        humidity: 20,
        distance: 80  // Example distance (adjust this value for testing)
    };

    function calculateBinPercentage(distance) {
        // Calculate the filling percentage (1 = full, 0 = empty)
        let percentage = 100-distance; // Return as a fixed 2 decimal number
        return percentage;
    }

    function fetchData() {
        document.getElementById('temp').textContent = data.temperature + ' °C';
        document.getElementById('hum').textContent = data.humidity + ' %';

        // Get the bin filling percentage
        const fillingPercentage = calculateBinPercentage(data.distance);
        document.getElementById('distance').textContent = fillingPercentage + ' %';

        const statusElement = document.getElementById('status');
        
        // Set the status based on the bin's filling percentage
        if (data.humidity > 65) {
            statusElement.textContent = 'Status: trash is rotten!';
            statusElement.className = 'status danger';
            const fullMessage = document.getElementById('fullMessage');
            fullMessage.style.display = 'block'; 
            // sendMessage('12345');
        } else {
            if (fillingPercentage < 25) {
                statusElement.textContent = 'Status: Safe';
                statusElement.className = 'status safe';
            } else if (fillingPercentage < 50) {
                statusElement.textContent = 'Status: Caution';
                statusElement.className = 'status warning';
            } else if (fillingPercentage < 75) {
                statusElement.textContent = 'Status: Danger';
                statusElement.className = 'status danger';
                
            } else  {
                statusElement.textContent = 'Status: Full';
                statusElement.className = 'status danger';
                const fullMessage = document.getElementById('fullMessage');
                fullMessage.style.display = 'block'; 
                // sendMessage('12345');
            }
        }

    }

            // Create a function to send the message https://api.callmebot.com/whatsapp.php?phone=&text=This+is+a+test&apikey=
function sendMessage(trashCanId) {
  const message = `🚨⚠️ Attention: The trash bin with ID: ${trashCanId} is FULL! 🗑️💥 Please collect the trash as soon as possible! 👷‍♂️💨 Thank you! 🙏👍`;

  // Send the GET request with the beautified message
  fetch(`https://api.callmebot.com/whatsapp.php?phone=94729917072&text=${encodeURIComponent(message)}&apikey=1627161`)
    .then(response => response.json())
    .then(data => {
      console.log('Message sent successfully:', data);
    })
    .catch(error => {
      console.error('Error sending message:', error);
    });
}


    

// Call the function with a sample trash can ID

    setInterval(fetchData, 500000); // Update every 5 seconds
    window.onload = fetchData;    // Load initial data
  </script>
</body>
</html>
