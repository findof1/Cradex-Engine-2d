function setup()
  obj1 = createGameObject()
  bg = createGameObject(1, -0.095, -0.14, 0.0, 11.0, 5.915, 1)
end

setup()

function update(deltaTime)

  local speed = 2.0 
  local x, y = getGameObjectPosition(obj1)
  local newX = x;
  local newY = y;
  local speed = 3 * deltaTime
  for k, v in pairs(pressedKeys) do
    if(v == 65) then
      newX = newX - speed;
    end

    if(v == 68) then
      newX = newX + speed;
    end 

    if(v == 87) then
      newY = newY + speed;
    end 

    if(v == 83) then
      newY = newY - speed;
    end 
    
  end
  setGameObjectPosition(obj1, newX, newY)
  drawGameObject(bg);
  drawGameObject(obj1);
end

