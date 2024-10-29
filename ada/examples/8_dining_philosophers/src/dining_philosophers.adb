with Ada.Text_IO; use Ada.Text_IO;
with Ada.Calendar; use Ada.Calendar;

procedure Dining_Philosophers is

   type Philosopher_Index is range 1 .. 5;
   Num_Philosophers : constant Integer := 5;

   protected Fork is
      entry Pick_Up;
      entry Put_Down;
   private
      Is_Occupied : Boolean := False;
   end Fork;

   protected body Fork is
      entry Pick_Up when not Is_Occupied is
      begin
         Is_Occupied := True;
      end Pick_Up;

      entry Put_Down when Is_Occupied is
      begin
         Is_Occupied := False;
      end Put_Down;
   end Fork;

   Forks : array(Philosopher_Index) of Fork;

   task type Philosopher is
      entry Start;
   end Philosopher;

   task body Philosopher is
      Id : Philosopher_Index;
   begin
      Id := Philosopher_Index'Val (Self'Address mod Num_Philosophers + 1);
      loop
         Think;
         Forks (Id).Pick_Up;
         Forks (Philosopher_Index ((Id mod Num_Philosophers) + 1)).Pick_Up;
         Eat;
         Forks (Id).Put_Down;
         Forks (Philosopher_Index ((Id mod Num_Philosophers) + 1)).Put_Down;
      end loop;
   end Philosopher;

   procedure Eat is
   begin
      Put_Line ("Philosopher "
        & Integer'Image(Self'Address mod Num_Philosophers + 1)
        & " is eating.");
      delay 1.0;
   end Eat;

   procedure Think is
   begin
      Put_Line ("Philosopher "
        & Integer'Image (Self'Address mod Num_Philosophers + 1)
        & " is thinking.");
      delay 1.0;
   end Think;

   Philosophers : array (Philosopher_Index) of Philosopher;

begin
   for I in Philosopher_Index loop
      Philosophers (I).Start;
   end loop;

   delay 10.0;

end Dining_Philosophers;
