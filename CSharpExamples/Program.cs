//using System;
using System.Windows;

namespace CSharpExamples
{
    using System;
    class Person
    {
        private string m_name = "No Name Yet";
        private string m_birthDate = null;

        public Person(string name)
        {
            Name = name;
        }
        public Person(string name, string birthDate):this(name)
        {
            m_birthDate = birthDate;
        }

        public string Name {
            get => m_name;
            private set => m_name = value;
        }

        public string BirthDate
        {
            get => m_birthDate;
            private set => m_birthDate = value;
        }

    }

    class Program
    {
        static void Main(string[] args)
        {
            var p = new Person("Jonas");
            //p.Name = "New Name";
            Console.WriteLine(p.Name);

        }
    }
}
