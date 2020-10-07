#include <vsg/io/ReaderWriter_vsg.h>
static auto text_frag = []() {std::istringstream str(
"#vsga 0.0.2\n\
Root id=1 vsg::ShaderStage\n\
{\n\
  NumUserObjects 0\n\
  Stage 16\n\
  EntryPoint \"main\"\n\
  ShaderModule id=2 vsg::ShaderModule\n\
  {\n\
    NumUserObjects 0\n\
    Source \"#version 450\n\
\n\
layout(binding = 0) uniform sampler2D texSampler;\n\
\n\
layout(location = 0) in vec4 fragColor;\n\
layout(location = 1) in vec2 fragTexCoord;\n\
\n\
layout(location = 0) out vec4 outColor;\n\
\n\
float glyph_alpha(vec2 texcoord, vec2 dx, vec2 dy)\n\
{\n\
    float scale = 1.0;\n\
    float distance_from_edge = (textureGrad(texSampler, texcoord, dx, dy).r - 0.5);\n\
\n\
    float d_distance_dx = dFdx(distance_from_edge) * scale;\n\
    float d_distance_dy = dFdy(distance_from_edge) * scale;\n\
\n\
    float delta = sqrt(d_distance_dx * d_distance_dx + d_distance_dy * d_distance_dy);\n\
\n\
    float min_distance_from_edge = distance_from_edge - delta;\n\
    float max_distance_from_edge = distance_from_edge + delta;\n\
\n\
    if (min_distance_from_edge >= 0.0) return 1.0;\n\
    else if (max_distance_from_edge >= 0.0) return max_distance_from_edge/(max_distance_from_edge-min_distance_from_edge);\n\
    return 0.0;\n\
}\n\
\n\
float sampled_glyph_alpha_grid(vec2 texcoord)\n\
{\n\
    float lod = textureQueryLod(texSampler, texcoord).x;\n\
    vec2 dx = dFdx(texcoord);\n\
    vec2 dy = dFdy(texcoord);\n\
\n\
    if (lod<=0.0) return glyph_alpha(texcoord, dx, dy);\n\
\n\
    float area = length(dx) * length(dy);\n\
    float average_side = sqrt(area) / (1.0 +lod);\n\
    float num_x = ceil(length(dx) / average_side);\n\
    float num_y = ceil(length(dy) / average_side);\n\
\n\
    vec2 interval_dx = dx / num_x;\n\
    vec2 interval_dy = dy / num_y;\n\
\n\
    float total_alpha = 0.0;\n\
    vec2 tc_row_start = texcoord - dx*0.5 - dy*0.5;\n\
    for(float r = 0; r<num_y; ++r)\n\
    {\n\
        vec2 tc = tc_row_start;\n\
        tc_row_start = tc_row_start + interval_dy;\n\
\n\
        for(float r = 0; r<num_x; ++r)\n\
        {\n\
            total_alpha = total_alpha + glyph_alpha(tc, interval_dx, interval_dy);\n\
\n\
            tc = tc + interval_dx;\n\
        }\n\
    }\n\
\n\
    return total_alpha / (num_x * num_y);\n\
}\n\
\n\
void main()\n\
{\n\
    outColor = vec4(fragColor.rgb,  fragColor.a * sampled_glyph_alpha_grid(fragTexCoord));\n\
\n\
    //outColor = fragColor;\n\
    if (outColor.a == 0.0) discard;\n\
}\n\
\"\n\
    SPIRVSize 1313\n\
    SPIRV 119734787 65536 524298 225 0 131089 1 131089 50 393227 1 1280527431\n\
     1685353262 808793134 0 196622 0 1 524303 4 4 1852399981 0 198\n\
     200 209 196624 4 7 196611 2 450 262149 4 1852399981 0\n\
     589829 13 1887005799 1818320744 677472368 993158774 993158774 993158774 0 327685 10 1668834676\n\
     1685221231 0 196613 11 30820 196613 12 31076 655365 17 1886216563 1600415084\n\
     1887005799 1818320744 1600219248 1684632167 845575720 59 327685 16 1668834676 1685221231 0 262149\n\
     20 1818321779 101 458757 22 1953720676 1701015137 1869768287 1684365165 25959 327685 26\n\
     1400399220 1819307361 29285 393221 38 1768185700 1851880563 1683973475 120 393221 43 1768185700\n\
     1851880563 1683973475 121 262149 48 1953260900 97 524293 57 1601071469 1953720676 1701015137\n\
     1869768287 1684365165 25959 524293 61 1601724781 1953720676 1701015137 1869768287 1684365165 25959 196613\n\
     85 6582124 196613 90 30820 196613 93 31076 262149 100 1634886000 109\n\
     262149 102 1634886000 109 262149 104 1634886000 109 262149 108 1634038369 0\n\
     393221 114 1919252065 1600481121 1701079411 0 262149 120 1601009006 120 262149 126\n\
     1601009006 121 327685 132 1702129257 1818326642 7890015 327685 137 1702129257 1818326642 7955551\n\
     327685 142 1635020660 1818320748 6383728 393221 143 1918854004 1935636335 1953653108 0 196613\n\
     151 114 196613 160 25460 196613 165 114 262149 175 1634886000 109\n\
     262149 177 1634886000 109 262149 179 1634886000 109 327685 198 1131705711 1919904879\n\
     0 327685 200 1734439526 1869377347 114 393221 209 1734439526 1131963732 1685221231 0\n\
     262149 210 1634886000 109 262215 26 34 0 262215 26 33 0\n\
     262215 198 30 0 262215 200 30 0 262215 209 30 1\n\
     131091 2 196641 3 2 196630 6 32 262167 7 6 2\n\
     262176 8 7 7 393249 9 6 8 8 8 262177 15\n\
     6 8 262176 19 7 6 262187 6 21 1065353216 589849 23\n\
     6 1 0 0 0 1 0 196635 24 23 262176 25\n\
     0 24 262203 25 26 0 262167 31 6 4 262165 33\n\
     32 0 262187 33 34 0 262187 6 36 1056964608 262187 6\n\
     66 0 131092 67 262176 197 3 31 262203 197 198 3\n\
     262176 199 1 31 262203 199 200 1 262167 201 6 3\n\
     262187 33 204 3 262176 205 1 6 262176 208 1 7\n\
     262203 208 209 1 262176 218 3 6 327734 2 4 0\n\
     3 131320 5 262203 8 210 7 262205 31 202 200 524367\n\
     201 203 202 202 0 1 2 327745 205 206 200 204\n\
     262205 6 207 206 262205 7 211 209 196670 210 211 327737\n\
     6 212 17 210 327813 6 213 207 212 327761 6 214\n\
     203 0 327761 6 215 203 1 327761 6 216 203 2\n\
     458832 31 217 214 215 216 213 196670 198 217 327745 218\n\
     219 198 204 262205 6 220 219 327860 67 221 220 66\n\
     196855 223 0 262394 221 222 223 131320 222 65788 131320 223\n\
     65789 65592 327734 6 13 0 9 196663 8 10 196663 8\n\
     11 196663 8 12 131320 14 262203 19 20 7 262203 19\n\
     22 7 262203 19 38 7 262203 19 43 7 262203 19\n\
     48 7 262203 19 57 7 262203 19 61 7 196670 20\n\
     21 262205 24 27 26 262205 7 28 10 262205 7 29\n\
     11 262205 7 30 12 524376 31 32 27 28 4 29\n\
     30 327761 6 35 32 0 327811 6 37 35 36 196670\n\
     22 37 262205 6 39 22 262351 6 40 39 262205 6\n\
     41 20 327813 6 42 40 41 196670 38 42 262205 6\n\
     44 22 262352 6 45 44 262205 6 46 20 327813 6\n\
     47 45 46 196670 43 47 262205 6 49 38 262205 6\n\
     50 38 327813 6 51 49 50 262205 6 52 43 262205\n\
     6 53 43 327813 6 54 52 53 327809 6 55 51\n\
     54 393228 6 56 1 31 55 196670 48 56 262205 6\n\
     58 22 262205 6 59 48 327811 6 60 58 59 196670\n\
     57 60 262205 6 62 22 262205 6 63 48 327809 6\n\
     64 62 63 196670 61 64 262205 6 65 57 327870 67\n\
     68 65 66 196855 70 0 262394 68 69 72 131320 69\n\
     131326 21 131320 72 262205 6 73 61 327870 67 74 73\n\
     66 196855 76 0 262394 74 75 76 131320 75 262205 6\n\
     77 61 262205 6 78 61 262205 6 79 57 327811 6\n\
     80 78 79 327816 6 81 77 80 131326 81 131320 76\n\
     131321 70 131320 70 131326 66 65592 327734 6 17 0 15\n\
     196663 8 16 131320 18 262203 19 85 7 262203 8 90\n\
     7 262203 8 93 7 262203 8 100 7 262203 8 102\n\
     7 262203 8 104 7 262203 19 108 7 262203 19 114\n\
     7 262203 19 120 7 262203 19 126 7 262203 8 132\n\
     7 262203 8 137 7 262203 19 142 7 262203 8 143\n\
     7 262203 19 151 7 262203 8 160 7 262203 19 165\n\
     7 262203 8 175 7 262203 8 177 7 262203 8 179\n\
     7 262205 24 86 26 262205 7 87 16 327785 7 88\n\
     86 87 327761 6 89 88 0 196670 85 89 262205 7\n\
     91 16 262351 7 92 91 196670 90 92 262205 7 94\n\
     16 262352 7 95 94 196670 93 95 262205 6 96 85\n\
     327868 67 97 96 66 196855 99 0 262394 97 98 99\n\
     131320 98 262205 7 101 16 196670 100 101 262205 7 103\n\
     90 196670 102 103 262205 7 105 93 196670 104 105 458809\n\
     6 106 13 100 102 104 131326 106 131320 99 262205 7\n\
     109 90 393228 6 110 1 66 109 262205 7 111 93\n\
     393228 6 112 1 66 111 327813 6 113 110 112 196670\n\
     108 113 262205 6 115 108 393228 6 116 1 31 115\n\
     262205 6 117 85 327809 6 118 21 117 327816 6 119\n\
     116 118 196670 114 119 262205 7 121 90 393228 6 122\n\
     1 66 121 262205 6 123 114 327816 6 124 122 123\n\
     393228 6 125 1 9 124 196670 120 125 262205 7 127\n\
     93 393228 6 128 1 66 127 262205 6 129 114 327816\n\
     6 130 128 129 393228 6 131 1 9 130 196670 126\n\
     131 262205 7 133 90 262205 6 134 120 327760 7 135\n\
     134 134 327816 7 136 133 135 196670 132 136 262205 7\n\
     138 93 262205 6 139 126 327760 7 140 139 139 327816\n\
     7 141 138 140 196670 137 141 196670 142 66 262205 7\n\
     144 16 262205 7 145 90 327822 7 146 145 36 327811\n\
     7 147 144 146 262205 7 148 93 327822 7 149 148\n\
     36 327811 7 150 147 149 196670 143 150 196670 151 66\n\
     131321 152 131320 152 262390 154 155 0 131321 156 131320 156\n\
     262205 6 157 151 262205 6 158 126 327864 67 159 157\n\
     158 262394 159 153 154 131320 153 262205 7 161 143 196670\n\
     160 161 262205 7 162 143 262205 7 163 137 327809 7\n\
     164 162 163 196670 143 164 196670 165 66 131321 166 131320\n\
     166 262390 168 169 0 131321 170 131320 170 262205 6 171\n\
     165 262205 6 172 120 327864 67 173 171 172 262394 173\n\
     167 168 131320 167 262205 6 174 142 262205 7 176 160\n\
     196670 175 176 262205 7 178 132 196670 177 178 262205 7\n\
     180 137 196670 179 180 458809 6 181 13 175 177 179\n\
     327809 6 182 174 181 196670 142 182 262205 7 183 160\n\
     262205 7 184 132 327809 7 185 183 184 196670 160 185\n\
     131321 169 131320 169 262205 6 186 165 327809 6 187 186\n\
     21 196670 165 187 131321 166 131320 168 131321 155 131320 155\n\
     262205 6 188 151 327809 6 189 188 21 196670 151 189\n\
     131321 152 131320 154 262205 6 190 142 262205 6 191 120\n\
     262205 6 192 126 327813 6 193 191 192 327816 6 194\n\
     190 193 131326 194 65592\n\
  }\n\
  NumSpecializationConstants 0\n\
}\n\
");
vsg::ReaderWriter_vsg io;
return io.read_cast<vsg::ShaderStage>(str);
};
