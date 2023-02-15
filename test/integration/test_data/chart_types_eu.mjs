function Rand(a) {
  return function() {
    var t = a += 0x6D2B79F5;
    t = Math.imul(t ^ t >>> 15, t | 1);
    t ^= t + Math.imul(t ^ t >>> 7, t | 61);
    return ((t ^ t >>> 14) >>> 0) / 4294967296;
  }
}

let rand = Rand(1234);

let countries = ['Austria', 'Belgium', 'Bulgaria', 'Cyprus', 'Czechia',
'Germany', 'Denmark', 'Estonia', 'Greece', 'Spain', 'Finland', 'France',
'Croatia', 'Hungary', 'Ireland', 'Italy', 'Lithuania', 'Luxembourg',
'Latvia', 'Malta', 'Netherlands', 'Poland', 'Portugal', 'Romania',
'Sweden', 'Slovenia', 'Slovakia', 'UK'];

let codes = ['AT', 'BE', 'BG', 'CY', 'CZ', 'DE', 'DK', 'EE', 'EL', 'ES', 'FI', 
'FR', 'HR', 'HU', 'IE', 'IT', 'LT', 'LU', 'LV', 'MT', 'NL', 'PL', 'PT', 'RO',
'SE', 'SI', 'SK', 'UK'];

let countriesData = new Array(28*7*4*3);
let codeData = new Array(28*7*4*3);

for (let i = 0; i < 12; i++)
  for (let j = 0; j < countries.length; j++)
    for (let k = 0; k < 7; k++)
{
  countriesData[ (((i*28)+j)*7)+k ] = countries[j];
  codeData[ (((i*28)+j)*7)+k ] = codes[j];
}

let yearsData = [];
for (let n = 0; n < 4; n++) {
  for (let i = 0; i < 28; i++) yearsData.push('01', '02', '03', '04', '05', '06', '07');
  for (let i = 0; i < 28; i++) yearsData.push('08', '09', '10', '11', '12', '13', '14');
  for (let i = 0; i < 28; i++) yearsData.push('15', '16', '17', '18', '19', '20', '21');  
}

let posNums = [
  6,  8,  16, 17, 8,  10, 4,  22, 19, 28, 28, 23, 29, 17, 20, 24, 39,  22,  29, 49, 13, 3,  8,  9,  10, 11, 5,  1,  12, 25, 24, 33, 22, 37, 5,   53,  78, 40, 20, 34, 37, 30, 38, 23, 24, 32, 27, 24, 34, 31, 27, 25, 21,  21,  28, 16, 2,  11, 11, 7,  7,  7,  1,  16, 23, 20, 27, 14, 17, 14, 29,  14,  27, 33, 28, 26, 17, 42, 32, 38, 44, 32, 38, 21, 15, 25, 16, 17, 14,  30,  9,  10, 12, 18, 13, 24, 22, 9,  29, 28, 34, 28, 25, 30, 12, 10, 19,  17,  22, 17, 22, 8,  21, 20, 1,  23, 13, 12, 10, 7,  15, 13, 11, 11, 12,  5,
  21, 23, 20, 20, 19, 36, 1,  12, 5,  2,  11, 8,  10, 3,  42, 27, 35,  56,  29, 51, 5,  32, 24, 35, 19, 26, 30, 10, 21, 28, 24, 31, 28, 18, 13,  15,  27, 23, 25, 15, 16, 2,  15, 23, 14, 26, 38, 26, 21, 19, 28, 11, 14,  14,  15, 10, 25, 19, 16, 19, 16, 15, 8,  50, 40, 42, 56, 43, 30, 34, 16,  15,  28, 35, 31, 34, 21, 12, 13, 9,  21, 14, 19, 10, 15, 26, 15, 25, 23,  28,  9,  1,  3,  7,  12, 7,  8,  3,  25, 46, 46, 46, 43, 55, 22, 68, 113, 19,  31, 43, 51, 40, 12, 17, 16, 16, 13, 20, 17, 6,  22, 42, 35, 28, 30,  21,
  22, 22, 26, 23, 21, 19, 12, 3,  7,  18, 14, 10, 12, 7,  14, 13, 15,  24,  18, 20, 9,  6,  11, 6,  13, 13, 6,  10, 6,  14, 14, 11, 10, 10, 7,   34,  40, 43, 41, 42, 54, 20, 18, 19, 29, 17, 15, 18, 9,  26, 27, 32, 43,  44,  66, 18, 13, 13, 12, 15, 13, 8,  10, 9,  8,  11, 11, 2,  7,  3,  10,  13,  18, 17, 19, 29, 0,  21, 36, 45, 33, 37, 27, 20, 6,  10, 18, 10, 11,  24,  1,  12, 24, 41, 26, 25, 17, 13, 7,  8,  10, 6,  4,  11, 7,  12, 7,   13,  6,  15, 10, 2,  9,  3,  4,  4,  7,  10, 6,  16, 30, 29, 33, 38, 39,  18,
  28, 39, 35, 39, 35, 28, 10, 8,  24, 13, 25, 13, 7,  16, 23, 13, 19,  18,  18, 14, 14, 25, 21, 13, 18, 12, 8,  10, 15, 25, 19, 22, 22, 30, 8,   24,  19, 21, 29, 15, 12, 9,  10, 11, 3,  9,  9,  4,  0,  20, 34, 9,  9,   13,  11, 12, 12, 1,  15, 4,  7,  2,  4,  3,  10, 17, 12, 15, 13, 9,  42,  59,  51, 43, 50, 51, 26, 27, 27, 35, 37, 27, 20, 12, 19, 8,  11, 6,  5,   8,  7,  23, 9,  26, 12, 17, 15, 10, 45, 41, 37, 40, 40, 47, 23, 6,  8,   24,  15, 12, 11, 2,  15, 18, 27, 23, 11, 15, 7,  37, 45, 38, 46, 35, 40,  24,
  24, 17, 13, 35, 27, 32, 20, 14, 4,  9,  12, 4,  5,  5,  21, 26, 23,  29,  21, 40, 1,  2,  3,  1,  1,  3,  4,  0,  3,  4,  3,  3,  4,  6,  0,   19,  15, 9,  9,  20, 11, 6,  24, 48, 51, 52, 43, 43, 16, 21, 18, 22, 18,  26,  19, 3,  0,  2,  4,  1,  0,  4,  1,  21, 19, 26, 19, 14, 13, 2,  33,  35,  27, 21, 26, 21, 6,  19, 18, 10, 12, 8,  6,  5,  20, 26, 22, 31, 14,  23,  8,  11, 23, 27, 20, 15, 19, 13, 25, 39, 31, 37, 36, 45, 17, 12, 19,  17,  11, 10, 16, 7,  18, 10, 4,  8,  5,  4,  8,  53, 46, 33, 20, 33, 36,  17,
  11, 10, 9,  13, 7,  5,  5,  11, 14, 15, 11, 13, 9,  9,  25, 28, 41,  46,  34, 59, 35, 21, 28, 26, 25, 16, 15, 10, 19, 30, 20, 16, 23, 20, 22,  13,  13, 16, 20, 18, 14, 13, 19, 33, 41, 28, 45, 48, 32, 26, 22, 20, 21,  23,  17, 12, 19, 14, 24, 20, 17, 9,  7,  5,  11, 16, 19, 7,  19, 15, 28,  42,  48, 63, 55, 37, 38, 5,  15, 15, 11, 14, 13, 4,  13, 19, 12, 13, 20,  26,  3,  3,  0,  4,  2,  3,  7,  1,  12, 13, 19, 29, 21, 20, 1,  8,  13,  15,  18, 22, 19, 11, 22, 33, 39, 43, 42, 37, 26, 15, 26, 41, 25, 20, 29,  16,
  21, 23, 18, 17, 17, 7,  7,  10, 32, 33, 26, 26, 30, 20, 17, 21, 22,  14,  21, 19, 7,  22, 12, 17, 15, 17, 7,  4,  7,  8,  7,  3,  4,  4,  4,   1,  2,  7,  0,  2,  2,  1,  9,  0,  5,  4,  2,  2,  0,  2,  2,  2,  0,   0,  0,  0,  6,  3,  0,  4,  1,  0,  0,  9,  13, 4,  6,  9,  4,  7,  5,   10,  6,  14, 9,  10, 9,  8,  5,  6,  4,  6,  1,  3,  1,  0,  2,  2,  0,   1,  0,  1,  1,  1,  2,  1,  0,  0,  2,  7,  3,  6,  7,  7,  2,  2,  3,   1,  5,  1,  4,  1,  4,  3,  4,  5,  3,  0,  0,  2,  0,  3,  2,  4,  1,   2,
  0,  8,  18, 6,  7,  7,  5,  3,  3,  4,  6,  0,  1,  0,  3,  9,  3,   5,  1,  1,  2,  0,  3,  2,  2,  1,  1,  0,  8,  4,  3,  4,  5,  13, 0,   2,  2,  0,  3,  4,  0,  2,  6,  3,  1,  4,  5,  7,  0,  7,  5,  6,  2,   5,  3,  1,  4,  1,  4,  0,  0,  1,  0,  10, 7,  9,  3,  3,  7,  0,  3,   0,  2,  2,  2,  2,  1,  11, 2,  3,  4,  3,  5,  0,  2,  4,  4,  0,  2,   2,  0,  7,  7,  3,  4,  4,  5,  2,  3,  6,  7,  5,  4,  9,  1,  13, 6,   4,  12, 4,  9,  1,  8,  4,  6,  5,  8,  9,  6,  5,  7,  1,  3,  3,  3,   1,
  12, 17, 6,  17, 12, 19, 10, 27, 50, 22, 28, 12, 21, 16, 6,  3,  1,   3,  14, 6,  4,  18, 14, 7,  3,  10, 15, 9,  0,  4,  4,  1,  3,  6,  1,   4,  7,  3,  5,  2,  4,  1,  12, 5,  23, 20, 15, 13, 10, 3,  5,  11, 6,   6,  7,  6,  10, 9,  12, 13, 12, 12, 4,  7,  6,  11, 7,  3,  3,  3,  9,   3,  6,  4,  5,  5,  2,  1,  3,  5,  8,  5,  6,  3,  13, 10, 12, 13, 13,  11, 23, 3,  3,  6,  2,  2,  3,  3,  19, 18, 27, 24, 20, 30, 4,  2,  0,   0,  0,  0,  0,  0,  9,  7,  12, 9,  10, 12, 1,  21, 29, 15, 21, 19, 17,  9,  2,  1,  10, 7,  6,  2,  5,  5,  12, 12, 9,  11, 8,  5,  6,  17, 15,  21,
  22, 15, 16, 4,  0,  4,  3,  3,  1,  1,  14, 9,  16, 10, 12, 11, 2,   1,   8,  8,  13, 14, 7,  9,  13, 10, 10, 12, 8,  14, 4,  12, 16, 25, 15,  19,  12, 6,  1,  1,  2,  3,  2,  2,  2,  2,  2,  0,  4,  0,  4,  1,  2,   6,  3,  4,  6,  2,  2,  20, 52, 8,  24, 9,  17, 29, 30, 38, 39, 27, 24,  23,  13, 3,  4,  6,  2,  5,  3,  1,  0,  1,  1,  2,  1,  2,  0,  4,  11,  10,  6,  5,  5,  2,  22, 28, 27, 19, 13, 16, 12, 15, 9,  16, 15, 16, 10,  6,  8,  3,  4,  6,  7,  5,  5,  11, 9,  15, 8,  5,  4,  11, 12, 13, 28,  22,
  5,  9,  3,  2,  6,  5,  4,  7,  7,  0,  8,  0,  0,  0,  3,  1,  0,   7,   9,  12, 9,  8,  4,  4,  2,  3,  2,  1,  2,  7,  0,  6,  1,  1,  5,   1,  6,  1,  21, 21, 31, 21, 23, 26, 1,  0,  7,  10, 5,  5,  4,  1,  2,   4,  2,  6,  1,  3,  1,  6,  6,  9,  8,  11, 12, 2,  30, 54, 32, 32, 20,  24,  22, 4,  9,  4,  8,  5,  8,  3,  3,  8,  5,  8,  7,  9,  2,  12, 11,  9,  16, 3,  6,  5,  5,  5,  12, 4,  1,  0,  2,  4,  4,  4,  0,  7,  3,   0,  4,  4,  3,  7,  1,  0,  0,  3,  6,  0,  4,  2,  0,  0,  4,  6,  1,   4,
  5,  1,  0,  14, 11, 3,  3,  9,  6,  3,  1,  7,  6,  6,  7,  4,  2,   7,  8,  11, 2,  3,  7,  3,  7,  0,  2,  6,  0,  0,  0,  4,  3,  2,  0,   4,  3,  0,  0,  0,  5,  3,  2,  5,  0,  5,  5,  6,  4,  3,  1,  1,  5,   4,  3,  3,  5,  2,  0,  0,  7,  5,  6,  7,  2,  2,  9,  9,  7,  9,  6,   1,  2,  1,  5,  2,  3,  1,  0,  0,  2,  11, 4,  1,  1,  4,  4,  2,  2,   1,  1,  1,  2,  1,  5,  16, 9,  6,  9,  15, 2,  0,  0,  3,  4,  0,  0,   0,  3,  4,  9,  3,  7,  5,  0,  3,  2,  5,  2,  2,  4,  1,  3,  2,  3,   1,
  1,  2,  1,  14, 13, 12, 9,  15, 15, 2,  9,  4,  8,  6,  3,  9,  8,   8,   5,  6,  4,  5,  2,  1,  0,  5,  15, 4,  6,  4,  0,  5,  9,  4,  7,   0,  3,  1,  10, 10, 6,  16, 12, 9,  6,  5,  3,  2,  10, 8,  8,  5,  5,   18,  17, 13, 12, 6,  8,  2,  1,  1,  3,  2,  4,  2,  9,  4,  5,  6,  5,   10,  0,  37, 55, 10, 11, 21, 27, 20, 6,  5,  15, 14, 13, 11, 11, 8,  7,   5,  7,  6,  10, 5,  2,  6,  4,  7,  9,  3,  4,  3,  4,  2,  5,  4,  0,   0,  0,  7,  7,  17, 19, 17, 11, 1,  0,  5,  0,  4,  3,  3,  7,  13, 14,  15,
  14, 12, 0,  9,  4,  13, 9,  12, 12, 2,  13, 15, 20, 11, 11, 9,  5,   9,   8,  9,  10, 5,  7,  12, 15, 3,  4,  12, 4,  6,  9,  2,  1,  1,  1,   1,  2,  1,  3,  7,  2,  10, 8,  14, 0,  0,  5,  1,  4,  2,  2,  3,  0,   3,  12, 10, 9,  32, 0,  1,  10, 11, 6,  9,  11, 2,  9,  1,  5,  3,  6,   4,  7,  10, 6,  15, 5,  8,  6,  3,  3,  15, 13, 17, 8,  16, 13, 6,  13,  11,  11, 13, 10, 6,  8,  11, 7,  15, 13, 9,  1,  5,  5,  10, 2,  10, 4,   7,  8,  9,  8,  13, 10, 10, 8,  16, 17, 5,  10, 12, 12, 6,  3,  3,  8,   6,
  1,  4,  0,  4,  1,  5,  2,  0,  1,  0,  1,  4,  5,  6,  7,  5,  5,   43,  50, 26, 26, 25, 37, 21, 15, 3,  12, 16, 5,  8,  5,  8,  13, 13, 10,  7,  7,  3,  0,  3,  1,  3,  1,  4,  3,  5,  9,  14, 12, 9,  7,  0,  8,   8,  8,  6,  13, 9,  7,  6,  12, 5,  9,  8,  9,  5,  3,  6,  9,  7,  7,   12,  0,  18, 2,  17, 3,  16, 8,  8,  5,  8,  17, 7,  5,  3,  5,  5,  4,   6,  8,  5,  2,  4,  11, 2,  8,  1,  2,  2,  0,  10, 3,  10, 9,  8,  8,   3,  3,  9,  8,  3,  3,  2,  0,  6,  4,  2,  0,  2,  4,  0,  12, 14, 19,  23,
  18, 20, 1,  9,  5,  14, 8,  4,  4,  1,  6,  6,  9,  6,  10, 5,  1,   5,   10, 8,  15, 7,  6,  3,  9,  16, 16, 15, 11, 19, 7,  4,  8,  14, 5,   11,  9,  2,  6,  8,  11, 12, 6,  6,  1,  7,  7,  6,  9,  13, 9,  2,  8,   10,  11, 19, 11, 8,  10, 9,  16, 17, 23, 15, 18, 4,  2,  2,  9,  8,  13,  10,  1,  0,  1,  3,  0,  2,  1,  1,  9,  24, 8,  10, 19, 11, 3,  47, 57,  13,  6,  9,  42, 29, 3,  3,  12, 21, 10, 9,  4,  4,  13, 13, 16, 17, 3,   3,  2,  1,  1,  5,  1,  1,  1,  3,  3,  6,  9,  6,  2,  3,  0,  7,  8,   6,
  10, 7,  2,  6,  16, 19, 13, 5,  13, 14, 4,  2,  3,  6,  5,  0,  0,   6,   9,  10, 7,  5,  9,  3,  9,  8,  12, 16, 15, 9,  5,  1,  4,  4,  8,   5,  6,  0,  2,  6,  5,  7,  2,  1,  0,  1,  5,  2,  8,  6,  4,  1,  7,   4,  6,  4,  4,  4,  0,  8,  3,  6,  4,  2,  1,  1,  6,  9,  26, 31, 22,  37,  2,  7,  11, 19, 11, 6,  8,  3,  5,  2,  6,  4,  0,  6,  2,  3,  4,   7,  5,  4,  5,  0,  9,  12, 13, 12, 12, 20, 9,  6,  10, 13, 5,  9,  10,  5,  2,  12, 11, 7,  10, 8,  1,  7,  2,  10, 9,  7,  5,  4,  8,  10, 11,  19,
  11, 8,  10, 9,  16, 17, 23, 15, 18, 4,  2,  2,  9,  8,  13, 10, 1,   0,   1,  3,  0,  2,  1,  1,  9,  24, 8,  10, 19, 11, 3,  47, 57, 13, 6,   9,  42, 29, 3,  3,  12, 21, 10, 9,  4,  4,  13, 13, 16, 17, 3,  3,  2,   1,  1,  5,  1,  1,  1,  3,  3,  6,  9,  6,  2,  3,  0,  7,  8,  6,  10,  7,  2,  6,  16, 19, 13, 5,  13, 14, 4,  2,  3,  6,  5,  0,  0,  6,  9,   10,  7,  5,  9,  3,  9,  8,  12, 16, 15, 9,  5,  1,  4,  4,  8,  5,  6,   0,  2,  6,  5,  7,  2,  1,  0,  1,  5,  2,  8,  6,  4,  1,  7,  4,  6,   4,
  4,  4,  0,  8,  3,  6,  4,  2,  1,  1,  6,  9,  26, 31, 22, 37, 2,   7,  11, 19, 11, 6,  8,  3,  5,  2,  6,  4,  0,  6,  2,  3,  4,  7,  5,   4,  5,  0,  9,  12, 13, 12, 12, 20, 9,  6,  10, 13, 5,  9,  10, 5,  2,   12,  11, 7,  10, 8,  1,  7,  2,  10, 9,  7,  5,  4
];

let smallNums = [
  ... (new Array(28)).fill([ 0.1122,  0.0918,  0.0918,  0.0969,  0.0918,  0.0969,  0.0969 ]).flat(),
  ... (new Array(28)).fill([ 0.0566,  0.0721,  0.0721,  0.0772,  0.0824,  0.0875,  0.0875 ]).flat(),
  ... (new Array(28)).fill([ 0.0885,  0.0781,  0.0729,  0.0625,  0.0729,  0.0625,  0.0625 ]).flat(),
  ... (new Array(28)).fill([ 0.0691,  0.0642,  0.0691,  0.0691,  0.0592,  0.0642,  0.0691 ]).flat(),
  ... (new Array(28)).fill([ 0.0142,  0.0142,  0.0095,  0.0095,  0.0095,  0.0095,  0.0047 ]).flat(),
  ... (new Array(28)).fill([ 0.0306,   0.0408,  0.0306,  0.0357,  0.0357,  0.0357,  0.0408 ]).flat(),
  ... (new Array(28)).fill([ 0.0357,  0.0408,  0.0357,   0.0357,  0.0408,  0.0306,  0.0306 ]).flat(),
  ... (new Array(28)).fill([ 0.0189,  0.0252,  0.0189,  0.0126,  0.0126,  0.0126,  0.0063 ]).flat(),
  ... (new Array(28)).fill([ 0.0225,  0.0225,  0.0338,  0.0281,  0.0338,  0.0338,  0.0394 ]).flat(),
  ... (new Array(28)).fill([ 0.0388,  0.0388,  0.0388,  0.0388,  0.0388,  0.0333,  0.0222 ]).flat(),
  ... (new Array(28)).fill([ 0.0238,  0.0238,  0.0333,  0.0380,  0.0333,  0.0333,  0.0285 ]).flat(),
  0.2734,  0.25,  0.2707,  0.4756,  0.3620,  0.2545,  0.5,  0.2734,  0.375,  0.4640,  0.6486,  0.4827,  0.5515,  0.2083,  0.0781,  0.0312,  0.2320,  0.2594,  0.3218,  0.2121,  0.0833,  0,  0.0625,  0.1546,  0,  0.1609,  0.0424,  0.2083,  0.3125,  0.4687,  0.3093,  0.3027,  0.5632,  0.2969,  0.25,  0.4687,  0.3125,  0.2707,  0.1297,  0.1609,  0.5939,  0.5416,  0.0781,  
  0.0937,  0.3093,  0.5189,  0.2816,  0.3393,  0.1666,  0.1171,  0.2812,  0.3093,  0.5621,  0.4827,  0.1272,  0.1666,  0.0781,  0.0312,  0.0386,  0.1297,  0.0402,  0.0424,  0.0416,  0.1562,  0.0937,  0.1546,  0.2594,  0.2413,  0.1272,  0.2916,  0, 0.1562,  0.1933,  0.1297,  0.2816,  0.2121,  0.0833,  0.1953,  
  0.4375,  0.5027,  0.3891,  0.1609,  0.4666,  0.625,  0.1171,  0.0312,  0.0773,  0.1729,  0.1206,  0,  0,  0.1953,  0.25,  0.2320,  0.2162,  0.1206,  0.2969,  0.1666,  0.2734,  0.1875,  0.1933,  0.4324,  0.4827,  0.2969,  0.375,  0.0390,  0.0937,  0.1160,  0.2162,  0.1609,  0.1696,  0,  0.0781,  0.1562,  0.1546,  
  0.1729,  0.0804,  0.0424,  0,  0.0781,  0.1875,  0.1160,  0.3891,  0.3620,  0.2545,  0.1666,  0.2343,  0.0937,  0.1546,  0.1297,  0.1206,  0.0848,  0,  0.5078,  0.125,  0.3480,  0.2594,  0.0804, 0.0424,  0.0833,  0.1953,  0.25,  0.5414,  0.6918,  0.5632,  0.6363,  0.75,  0.2343,  0.2187,  0.3867,  0.3891,  0.1609,  0.2545, 
  0.2083,  0.1953,  0.0625,  0.1546,  0.1297,  0,  0.1696,  0.125, 0.0781,  0.0937,  0.1546,  0.1729,  0.1206,  0.1696,  0,  0.2734,0.2187,  0.3480,  0.3027,  0.3218,  0.5515,  0.3333,  0.1953,  0.1875,  0.3093,  0.1297,  0.2413,  0.2969,  0.3333,  0.0781,  0.2187,  0.2320,  0.1729,  0.2816,  0.2545,  0.125,  0.1953,  
  0.0312,  0.2707,  0.2162,  0.2413,  0.2121,  0.1666      
];

let hugeNums = smallNums.map(x => Math.floor(x * rand() * 1000000000));

let negNums = posNums.map(x => x * (rand() > 0.5 ? 1 : -1));
let negSmallNums = smallNums.map(x => x * (rand() > 0.5 ? 1 : -1));
let negHugeNums = hugeNums.map(x => x * (rand() > 0.5 ? 1 : -1));

export var data = {
  series: [
    { name: 'Country_code', type: 'dimension', values: codeData },
    { name: 'Country', type: 'dimension', values: countriesData },
    {
      name: 'Joy factors',
      type: 'dimension',
      values: [
        ... ( (new Array(588)).fill('Happiness')),
        ... ( (new Array(588)).fill('Creativity')),
        ... ( (new Array(588)).fill('Friendship')),
        ... ( (new Array(588)).fill('Love'))
      ]
    },
    { name: 'Year', type: 'dimension', values: yearsData },
    { name: 'Value 2 (+)', type: 'measure', values: posNums },
    { name: 'Value 1 (+)', type: 'measure', values: smallNums },
    { name: 'Value 3 (+)', type: 'measure', values: hugeNums },
    { name: 'Value 5 (+/-)', type: 'measure', values: negNums },
    { name: 'Value 4 (+/-)', type: 'measure', values: negSmallNums },
    { name: 'Value 6 (+/-)', type: 'measure', values: negHugeNums },
    { name: 'index', type: 'measure', values: (new Array(2352)).fill(1) }
  ]
};

export var data_3 = {
  series: data.series,
  filter: record =>
    record.Country == "Austria" ||
    record.Country == "Belgium" ||
    record.Country == "Bulgaria"
}

export var data_4 = {
  series: data.series,
  filter: record =>
    record.Country == "Bulgaria" ||
    record.Country == "Germany" ||
    record.Country == "Malta" ||
    record.Country == "Lithuania"
}

export var data_6 = {
  series: data.series,
  filter: record =>
    record.Country == "Austria" ||
    record.Country == "Belgium" ||
    record.Country == "Bulgaria" ||
    record.Country == "Cyprus" ||
    record.Country == "Czechia" ||
    record.Country == "Denmark"
}

export var data_14 = {
  series: data.series,
  filter: record =>
    record.Country == "Austria" ||
    record.Country == "Belgium" ||
    record.Country == "Bulgaria" ||
    record.Country == "Cyprus" ||
    record.Country == "Czechia" ||
    record.Country == "Denmark" ||
    record.Country == "Estonia" ||
    record.Country == "Greece" ||
    record.Country == "Germany" ||
    record.Country == "Spain" ||
    record.Country == "Finland" ||
    record.Country == "France" ||
    record.Country == "Croatia" ||
    record.Country == "Hungary"
}