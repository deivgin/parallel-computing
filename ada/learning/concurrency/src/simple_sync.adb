with Ada.Text_IO; use Ada.Text_IO;

package body Simple_Sync is
   task body T is
   begin
      for I in 1 .. 10 loop
         Put_Line ("hello");
      end loop;
   end T;
end Simple_Sync;
