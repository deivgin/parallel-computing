with Ada.Text_IO; use Ada.Text_IO;

procedure My_Hello_World is
   N : Integer := 0;
begin
   while N < 10 loop
      Put_Line ("Hello, World!!" & Integer'Image (N));
      N := N + 1;
   end loop;
end My_Hello_World;
