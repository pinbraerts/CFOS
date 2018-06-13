#ifndef IMAGE_H
#define IMAGE_H

#include "includes.h"
#define CHECK if (FAILED(hr)) throw std::runtime_error("HR Error")

class ImageBuilder {
private:
	IWICImagingFactory& imagingFactory;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* src = nullptr;
	IWICFormatConverter* converter = nullptr;
	IWICBitmapScaler* scaler = nullptr;
	IWICBitmapSource* res = nullptr;

public:
	ImageBuilder(LPCWSTR uri, IWICImagingFactory* imf): imagingFactory(*imf) {
		HRESULT hr;
		hr = imagingFactory.CreateDecoderFromFilename(
			uri, nullptr, GENERIC_READ,
			WICDecodeMetadataCacheOnLoad, &decoder
		);
		CHECK;

		hr = decoder->GetFrame(0, &src);
		CHECK;

		res = src;
	}

	ImageBuilder& convert(WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherType dType = WICBitmapDitherTypeNone,
		WICBitmapPaletteType pType = WICBitmapPaletteTypeMedianCut) {
		WICPixelFormatGUID pf;
		res->GetPixelFormat(&pf);
		if (pf != format) {
			HRESULT hr;
			hr = imagingFactory.CreateFormatConverter(&converter);
			CHECK;
			hr = converter->Initialize(res, format, dType, nullptr, 0.f, pType);
			CHECK;
			res = converter;
		}

		return *this;
	}

	ImageBuilder& scale(UINT dWidth, UINT dHeight = 0,
		WICBitmapInterpolationMode iMode = WICBitmapInterpolationModeLinear) {
		UINT w, h;
		res->GetSize(&w, &h);
		if ((dWidth != w || dHeight != h) && (dWidth != 0 && dHeight != 0)) {
			if (dWidth == 0)
				dWidth = w * dHeight / h;
			else if (dHeight == 0)
				dHeight = h * dWidth / w;
			HRESULT hr;
			hr = imagingFactory.CreateBitmapScaler(&scaler);
			CHECK;
			hr = scaler->Initialize(res, dWidth, dHeight, iMode);
			CHECK;
			res = scaler;
		}

		return *this;
	}

	HRESULT to(ID2D1RenderTarget* renderTarget, ID2D1Bitmap*& bmp) {
		return renderTarget->CreateBitmapFromWicBitmap(res, &bmp);
	}

	~ImageBuilder() {
		release(decoder);
		release(src);
		release(converter);
		release(scaler);
	}
};

#endif // !IMAGE_H
