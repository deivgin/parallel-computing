with Ada.Text_IO; use Ada.Text_IO;

procedure Reader_Writer is
   task Reader is
      entry Read_Data (Data : Integer);
   end Reader;
   task Writer;

   task body Reader is
   begin
      for I in 1 .. 5 loop
         accept Read_Data (Data : Integer) do
            Put_Line ("Reader: Received data = "
             & Integer'Image (Data));
         end Read_Data;
      end loop;
   end Reader;

   task body Writer is
      Data : Integer := 1;
   begin
      for I in 1 .. 5 loop
         delay 0.5;
         Put_Line ("Writer: Sending data = "
          & Integer'Image (Data));
         Reader.Read_Data (Data);
         Data := Data + 1;
      end loop;
   end Writer;

begin
   Put_Line ("Writer-Reader problem starts:");
   delay 3.0;
   Put_Line ("Writer-Reader problem ends.");
end Reader_Writer;
