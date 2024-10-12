with Ada.Text_IO; use Ada.Text_IO;
with Ada.Float_Text_IO; use Ada.Float_Text_IO;

procedure Temperature_Converter is
   Celsius_Temperature : Float;
   Fahrenheit_Temperature : Float;

   type Temperature_Array is array (1 .. 5) of Float;
   Fixed_Temperatures : Temperature_Array := (0.0, 20.0, 37.5, 15.0, 30.0);

   task Producer is
      entry Produce (Temp : out Float);
   end Producer;

   task Consumer is
      entry Consume (Temp : Float);
   end Consumer;

   task body Producer is
   begin
      for I in Fixed_Temperatures'Range loop
         accept Produce (Temp : out Float) do
            Celsius_Temperature := Fixed_Temperatures (I);
            Put ("Produced Temperature: ");
            Put (Celsius_Temperature, Fore => 1, Aft => 1, Exp => 0);
            Put_Line (" °C");
            Temp := Celsius_Temperature;
         end Produce;
      end loop;
   end Producer;

   task body Consumer is
   begin
      for I in Fixed_Temperatures'Range loop
         accept Consume (Temp : Float) do
            Fahrenheit_Temperature := (Temp * 9.0 / 5.0) + 32.0;
            Put ("Consumed Temperature: ");
            Put (Temp, Fore => 1, Aft => 1, Exp => 0);
            Put (" °C -> ");
            Put (Fahrenheit_Temperature, Fore => 1, Aft => 1, Exp => 0);
            Put_Line (" °F");
         end Consume;
      end loop;
   end Consumer;

begin
   for I in Fixed_Temperatures'Range loop
      declare
         Temp : Float;
      begin
         Producer.Produce (Temp);
         Consumer.Consume (Temp);
      end;
   end loop;

   Put_Line ("Temperature conversion completed.");
end Temperature_Converter;
