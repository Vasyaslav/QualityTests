using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.Threading;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.ConstrainedExecution;

class Program
{
    static Random rand = new Random();
    static void Main(string[] args)
    {
        Console.WriteLine("Input string length (positive integer): ");
        int length = Convert.ToInt32(Console.ReadLine());
        PictureFromString(generateRandomString(length));
        RandomString();
    }

    static string generateRandomString(int length)
    {
        const string digits = "0123456789";
        const string upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const string lowerLetters = "abcdefghijklmnopqrstuvwxyz";
        const string specialChars = "!#%:,.;*()[]{}<>/?@&-+= ";

        List<char> chars = new List<char>();

        // Adding at least 1 required symbol from every set of chars
        chars.Add(digits[rand.Next(digits.Length)]);
        chars.Add(upperLetters[rand.Next(upperLetters.Length)]);
        chars.Add(lowerLetters[rand.Next(lowerLetters.Length)]);
        chars.Add(specialChars[rand.Next(specialChars.Length)]);

        for (int i = 4; i < length; ++i)
        {
            int choice = rand.Next(4);
            switch (choice)
            {
                case 0:
                    chars.Add(digits[rand.Next(digits.Length)]);
                    break;
                case 1:
                    chars.Add(upperLetters[rand.Next(upperLetters.Length)]);
                    break;
                case 2:
                    chars.Add(lowerLetters[rand.Next(lowerLetters.Length)]);
                    break;
                case 3:
                    chars.Add(specialChars[rand.Next(specialChars.Length)]);
                    break;
            }
        }
        
        chars = Shuffle(chars);
        return new string(chars.ToArray());
    }

    static int RandomString()
    {
        int length;
        Console.WriteLine("Input string length (positive integer): ");
        length = Convert.ToInt32(Console.ReadLine());

        if (length <= 0)
        {
            Console.WriteLine("String length must be positive");
            return 1;
        }

        if (length < 4)
        {
            Console.WriteLine("Unable to create required string with length less than 4");
            return 1;
        }

        string randomString = generateRandomString(length);

        Console.WriteLine($"Generated string: {randomString}");

        return 0;
    }

    static public List<T> Shuffle<T>(List<T> list)
    {
        int n = list.Count;
        while (n > 1)
        {
            n--;
            int k = rand.Next(n + 1);
            T value = list[k];
            list[k] = list[n];
            list[n] = value;
        }
        return list;
    }

    static void AddGreenNoise(Bitmap bitmap)
    {
        Random random = new Random();
        for (int y = 0; y < bitmap.Height; y++)
        {
            for (int x = 0; x < bitmap.Width; x++)
            {
                int noise = random.Next(0, 50);
                Color pixel = bitmap.GetPixel(x, y);
                int green = Math.Min(255, pixel.G + noise); // Adding noise to green channel
                bitmap.SetPixel(x, y, Color.FromArgb(pixel.R, green, pixel.B));
            }
        }
    }

    static void PictureFromString(string text)
    {
        int width = 720;
        int height = 360;

        using (Bitmap bitmap = new Bitmap(width, height))
        {
            using (Graphics g = Graphics.FromImage(bitmap))
            {
                g.Clear(Color.Black);

                AddGreenNoise(bitmap);

                using (Pen yellowPen = new Pen(Color.Yellow, 0.75f))
                {
                    g.DrawLine(yellowPen, 0, height, width, 0);
                }

                FontFamily fontFamily = new FontFamily("Times New Roman");
                Color textColor = Color.Red;
                int fontSize = 24;

                // Adjusting font size
                SizeF textSize;
                do
                {
                    fontSize -= 2;
                    if (fontSize < 4)
                    {
                        Console.WriteLine("Текст слишком длинный для отображения!");
                        return;
                    }
                    using (Font font = new Font(fontFamily, fontSize, FontStyle.Regular, GraphicsUnit.Point))
                    {
                        textSize = g.MeasureString(text, font);
                    }
                } while (textSize.Width > width || textSize.Height > height);

                float posX = (width - textSize.Width) / 2;
                float posY = (height - textSize.Height) / 2;

                using (Font font = new Font(fontFamily, fontSize, FontStyle.Regular, GraphicsUnit.Point))
                using (SolidBrush brush = new SolidBrush(textColor))
                {
                    g.TextRenderingHint = TextRenderingHint.AntiAlias;
                    g.DrawString(text, font, brush, posX, posY);
                }
            }

            bitmap.Save("output.jpg", ImageFormat.Jpeg);
            Console.WriteLine("Изображение сохранено как output.jpg");
        }
    }
}