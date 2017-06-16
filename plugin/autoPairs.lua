function moveto(sx, sy)
  local cx = m:getCursorPosX()
  local cy = m:getCursorPosY()
  local ax = 0
  if ( cy < sy ) then
    repeat
      m:cursorDown()
      cy = m:getCursorPosY()
    until ( cy == sy )
  elseif ( cy > sy ) then
    repeat
      m:cursorUp()
      cy = m:getCursorPosY()
    until ( cy == sy )
  end
  m:cursorBeginLine()
  while( ax < sx ) do
    ax = ax + 1
    m:cursorRight()
  end
end

function autoPairs()
  pairs = "{}%[%]'\""
  if key ~= "(" and key ~= ")" and not string.find(pairs, key, nil, true) then
    return;
  end
  x, y = m:getCursorPosX(), m:getCursorPosY()

  if m:getIsSelected() then
    local sx, sy = m:getSelectionStartX(), m:getSelectionStartY()
    if ( sx < x ) or ( sy > sy ) then
      m:bkspcChar()
      if key == "(" then
        m:insertStdString(")")
      elseif key == "[" then
        m:insertStdString("]")
      elseif key == "{" then
        m:insertStdString("}")
      elseif key == "'" then
        m:insertStdString("'")
      elseif key == "\"" then
        m:insertStdString("\"")
      end
      moveto(sx, sy)
      if key == "(" then
        m:insertStdString("(")
      elseif key == "[" then
        m:insertStdString("[")
      elseif key == "{" then
        m:insertStdString("{")
      elseif key == "'" then
        m:insertStdString("'")
      elseif key == "\"" then
        m:insertStdString("\"")
      end
      moveto(x, y)
      m:cursorRight()
    else
      moveto(sx + 1, sy)
      if key == "(" then
        m:insertStdString(")")
      elseif key == "[" then
        m:insertStdString("]")
      elseif key == "{" then
        m:insertStdString("}")
      elseif key == "'" then
        m:insertStdString("'")
      elseif key == "\"" then
        m:insertStdString("\"")
      end
      moveto(x, y)
    end
  else
    if key == "(" then
      m:insertStdString(")")
      m:cursorLeft()
    elseif key == "[" then
      m:insertStdString("]")
      m:cursorLeft()
    elseif key == "{" then
      m:insertStdString("}")
      m:cursorLeft()
    elseif key == "'" then
      local nowline = m:getCurrentLine()
      x, y = m:getCursorPosX(), m:getCursorPosY()
      local comb = string.sub(nowline, x-1, x-1)..string.sub(nowline, x+1, x+1)
      if nowline:len() > x and x > 0 then
        if comb == "''" then
          m:deleteChar()
        else
          m:insertStdString("'")
          m:cursorLeft()
        end
      else
        m:insertStdString("'")
        m:cursorLeft()
      end
    elseif key == "\"" then
      local nowline = m:getCurrentLine()
      x, y = m:getCursorPosX(), m:getCursorPosY()
      local comb = string.sub(nowline, x-1, x-1)..string.sub(nowline, x+1, x+1)
      if nowline:len() > x and x > 0 then
        if comb == "\"\"" then
          m:deleteChar()
        else
          m:insertStdString("\"")
          m:cursorLeft()
        end
      else
        m:insertStdString("\"")
        m:cursorLeft()
      end
    elseif string.find(")}]'\"", key, nil, true) then
      local nowline = m:getCurrentLine()
      x, y = m:getCursorPosX(), m:getCursorPosY()
      local comb = string.sub(nowline, x-1, x-1)..string.sub(nowline, x+1, x+1)
      if nowline:len() > x and x > 0 then
        if comb == "()" or comb == "{}" or comb == "[]" then
          m:deleteChar()
          m:cursorRight()
        end
      end
    end
  end
end

autoPairs()
