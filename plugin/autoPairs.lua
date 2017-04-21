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
   pairs = "{['\""
   if key ~= "(" and not string.find(pairs, key) then
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
	 m:insertStdString("'")
	 m:cursorLeft()
      elseif key == "\"" then
	 m:insertStdString("\"")
	 m:cursorLeft()
      end
   end
end

autoPairs()
