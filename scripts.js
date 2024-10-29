var grid = document.getElementsByClassName("grid");

function generateTiles(){
  for(let i = 0; i < 121; i++){
      let tile = document.createElement("div");
      tile.classList.add('tile', `color-0`);
      grid[0].appendChild(tile);
  }
}

generateTiles()

async function getData() {
    const url = "http://localhost:3000/run_cpp.json";
    try {
      const response = await fetch(url);
      if (!response.ok) {
        throw new Error(`Response status: ${response.status}`);
      }
      const json = await response.json();
      buffer = json["grid"];
      header = document.getElementById('min-h');
      header.innerText = "Min = " + json["min"] + " Average = " + json["average"];
      element = document.getElementsByClassName("tile");
      for(let i = 0; i < 121; i++){
        classes = 'tile color-' + buffer[i];
        element[i].className = classes;
      }
    } catch (error) {
      console.error(error.message);
    }
  }

