# PPM and BMP image editor
This is a project I made for university in 2020. It's an editor for PPM and BMP images, which allows you to apply filters, mirror, and crop a picture.

---

## Building
Execute 'make' command in bash for building the app.

## Usage
The program is run with command line arguments, with a flag followed by an option or a file name if necessary.

### Input
To select the input file, use the '-input' flag, followed by the filename, as follows:

```bash
./ppm_bmp_filters -input [input_file_name]
```

It's important that the input file have .PPM or .BMP extention.

### Output
In a similar way, use the '-output' flag as follows:

```bash
-output [output_file_name]
```

### Filters
To apply a filter, use the '-filter' flag, followed by the filter name.

```bash
-filter [filter_name]
```

The following filters are available:
- invertir
- saturar
- gama
- brillo
- contraste
- mezclar
- sepia
- monocromo
- toaster
- valencia

Use example:

```bash
./ppm_bmp_filters -input [input_file_name] -output [output_file_name] -filter [filter_name]
```
