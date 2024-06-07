const char *CAPTIVE_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body {
      height: 100vh;
    }

    h1 {
      color: white;
      font-family: "Roboto", sans-serif;
    }

    main {
      display: flex;
      justify-content: center;
      align-items: center;
      flex-direction: column;
      gap: 5rem;
      background: radial-gradient(
        circle,
        rgba(2, 51, 89, 1) 33%,
        rgba(2, 0, 36, 1) 87%
      );
      height: 100%;
    }

    button {
      border: none;
      padding: 0.5rem 1rem 0.5rem 1rem;
      border-radius: 20px;
      color: #1e1e1e;
      background: lightblue;
    }
  </style>
  <body>
    <main>
      <h1>Welcome!</h1>
      <a href="/home"><button>Select Network</button></a>
    </main>
  </body>
</html>

)rawliteral";

const char *MAIN_PAGE_1 = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Access Point</title>
  <style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body { height: 100vh }
main {
  display: flex;
  justify-content: center;
  align-items:center;
  flex-direction: column;
  gap: 1rem;
  background: radial-gradient(circle, rgba(2,51,89,1) 33%, rgba(2,0,36,1) 87%);
  height: 100%;
}

button {
  border: none;
  padding: 0.5rem 1rem 0.5rem 1rem;
  border-radius: 20px;
  color: #1e1e1e;
  background: lightblue;
}

label { color: white }

section, form {
  display: flex;
  gap: 2rem;
  flex-direction: column;
  align-items: center;
}

input, select {
  padding: .2rem .5rem .2rem .5rem;
  border-radius: 10px;
  width: 100%; 
}
  </style>
</head>
<body>
<main>
  <form action="/connect" method="POST">
    <section>
      <label for="ssid">Wifi</label>
      <select id="ssid" name="ssid">
        <option value="">--- Select Network ---</option>
)rawliteral";

const char *MAIN_PAGE_2 = R"rawliteral(
          </select>
        </section>
        <section>
          <label for="password">Password</label>
          <input type="password" id="password" name="password" />
        </section>
        <button type="submit">Connect</button>
      </form>
    </main>
  </body>
</html>
)rawliteral";