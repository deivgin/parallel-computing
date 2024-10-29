with Ada.Text_IO; use Ada.Text_IO;
with Ada.Command_Line; use Ada.Command_Line;

procedure Command_Line_Arguments is
begin
  for Next in 1 .. Ada.Command_Line.Argument_Count loop
    Ada.Text_IO.Put_Line (Ada.Command_Line.Argument (Next));
  end loop;
end Command_Line_Arguments;
