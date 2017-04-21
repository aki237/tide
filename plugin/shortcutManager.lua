if modifier ~= "control" then
   return 0
end

alphanums = "# '\"[{}]\\|=+-_`~!@#$%^&*(),./?"

if key == 65363 then
   local line = m:getCurrentLine();
   local x = m:getCursorPosX();
   for i = x, line:len() do
      local a = string.sub(line, i+1, i+1);
      if string.find(alphanums, a) then
	 m:cursorRight()
      end
   end
end
