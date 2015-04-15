using System;
using System.Collections.Generic;
using System.Linq;

namespace Project_Euler_3
{
    delegate object factoring(UInt64 value);
    class Caster { public static T Cast<T>(object obj, T t) { return (T) obj; } }

    class PrimeFactor
    {
        private UInt64 value;
        private factoring next = (UInt64 _value) =>
        {
            UInt64 _factor = 2;
            while (0 != (_value % _factor)) { ++_factor; }
            return new { Factor = _factor, Value = _value / _factor };
        };

        public IEnumerable<UInt64> factor
        {
            get
            {
                while(true)
                {
                    var result = Caster.Cast(next(this.value), new { Factor = default(UInt64), Value = default(UInt64)});
                    yield return result.Factor;
                    if(result.Factor > result.Value) { yield break; }
                    this.value = result.Value;
                }
            }
        }

        public PrimeFactor(UInt64 value) { this.value = value; }
    }

    class Project_Euler_3
    {
        static void Main(string[] args)
        {
            Console.Write("Enter value: ");
            try { Console.WriteLine("Largest prime factor is {0}. Press <ENTER> to continue.", (new PrimeFactor(UInt64.Parse(Console.ReadLine()))).factor.Max()); }
            catch(FormatException) { Console.WriteLine("Bad number format!"); }
            catch (ArgumentNullException) { Console.WriteLine("Input empty!"); }
            while (!Console.KeyAvailable) { /* do nothing */ };
        }
    }
}
