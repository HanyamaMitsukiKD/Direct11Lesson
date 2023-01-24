#pragma once

// 最初と最後に緩急
static float EaseInOutSine(float progress)
{
	return (float)-(std::cos(M_PI * progress) - 1) / 2;
}

// 出が早くて最後にゆっくり
static float EaseOutQuad(float progress)
{
	return (float)1 - (1 - progress) * (1 - progress);
}

static float EaseOutCirc(float progress)
{
	return (float)sqrt(1 - pow(progress - 1, 2));
}